#include "AFAnimationChannel.h"

#include "AFMath.h"

AFAnimationChannel::~AFAnimationChannel()
{
	free(m_channelData.timings);
	free(m_channelData.values);
}

void AFAnimationChannel::LoadChannelData(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim,
                                         tinygltf::AnimationChannel channel)
{
	// Get important variables for the input (like keyframe times).
	const tinygltf::Accessor& inputAccessor = model->accessors.at(anim.samplers.at(channel.sampler).input);
	const tinygltf::BufferView& inputBufferView = model->bufferViews.at(inputAccessor.bufferView);
	const tinygltf::Buffer& inputBuffer = model->buffers.at(inputBufferView.buffer);

	// Get important variables for output (like translation and rotation).
	const tinygltf::Accessor& outputAccessor = model->accessors.at(anim.samplers.at(channel.sampler).output);
	const tinygltf::BufferView& outputBufferView = model->bufferViews.at(outputAccessor.bufferView);
	const tinygltf::Buffer& outputBuffer = model->buffers.at(outputBufferView.buffer);

	// Get keyframe count.
	const uint32_t keyCount = static_cast<uint32_t>(outputAccessor.count);
	m_channelData.keyCount = keyCount;

	// Get joint for this channel.
	m_channelData.targetJoint = static_cast<uint16_t>(channel.target_node);

	// Allocate enough memory for timings and values arrays.
	m_channelData.timings = static_cast<float*>(std::malloc(sizeof(float) * keyCount));
	m_channelData.values = static_cast<glm::vec4*>(std::malloc(sizeof(glm::vec4) * keyCount));

	// Get timings for every frame.
	std::memcpy(m_channelData.timings, &inputBuffer.data.at(0) + inputBufferView.byteOffset, inputBufferView.byteLength);

	// Get interpolation type between frames.
	const tinygltf::AnimationSampler sampler = anim.samplers.at(channel.sampler);
	if(sampler.interpolation == "STEP")
	{
		m_channelData.interpType = EAFInterpolationType::Step;
	}
	else if(sampler.interpolation == "LINEAR")
	{
		m_channelData.interpType = EAFInterpolationType::Linear;
	}
	else if (sampler.interpolation == "CUBICSPLINE")
	{
		m_channelData.interpType = EAFInterpolationType::CubicSpline;
	}

	// Get target path for this channel.
	if(channel.target_path == "translation")
	{
		m_channelData.targetPath = EAFTargetPath::Translation;
	}
	else if (channel.target_path == "rotation")
	{
		m_channelData.targetPath = EAFTargetPath::Rotation;
	}
	else if (channel.target_path == "scale")
	{
		m_channelData.targetPath = EAFTargetPath::Scale;
	}

	// Get values for this channel.
	const uint8_t* rawValues = &outputBuffer.data.at(0) + outputBufferView.byteOffset;
	if (outputAccessor.type == TINYGLTF_TYPE_VEC3) 
	{
		const glm::vec3* src = reinterpret_cast<const glm::vec3*>(rawValues);
		for (size_t i = 0; i < outputAccessor.count; ++i)
		{
			m_channelData.values[i] = glm::vec4(src[i], 0.0f); // Copy with padding w = 0.
		}
	}
	else if (outputAccessor.type == TINYGLTF_TYPE_VEC4)
	{
		const glm::vec4* src = reinterpret_cast<const glm::vec4*>(rawValues);
		std::memcpy(m_channelData.values, src, sizeof(glm::vec4) * outputAccessor.count);
	}
}

void AFAnimationChannel::LoadChannelData(const FAFAnimationChannelData& channelData)
{
	m_channelData = channelData;
}

const FAFAnimationChannelData& AFAnimationChannel::GetChannelData() const
{
	return m_channelData;
}

int AFAnimationChannel::GetTargetJoint() const
{
	return m_channelData.targetJoint;
}

EAFTargetPath AFAnimationChannel::GetTargetPath() const
{
	return m_channelData.targetPath;
}

glm::vec3 AFAnimationChannel::GetTranslation(float time) const
{
	if (!m_channelData.values)
	{
		return glm::vec3(0.0f);
	}

	if (time < m_channelData.timings[0])
	{
		return glm::vec3(m_channelData.values[0]);
	}

	if (time > m_channelData.timings[m_channelData.keyCount - 1])
	{
		return glm::vec3(m_channelData.values[m_channelData.keyCount - 1]);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for (uint32_t i = 0; i < m_channelData.keyCount; ++i)
	{
		if (m_channelData.timings[i] > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if (prevTimeIndex == nextTimeIndex)
	{
		return glm::vec3(m_channelData.values[prevTimeIndex]);
	}

	glm::vec3 finalTranslation = glm::vec3(0.0f);

	switch (m_channelData.interpType)
	{
	case EAFInterpolationType::Step:
	{
		finalTranslation = glm::vec3(m_channelData.values[prevTimeIndex]);
		break;
	}
	case EAFInterpolationType::Linear:
	{
		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const glm::vec3& prevTranslation = glm::vec3(m_channelData.values[prevTimeIndex]);
		const glm::vec3& nextTranslation = glm::vec3(m_channelData.values[nextTimeIndex]);

		finalTranslation = prevTranslation + interpolatedTime * (nextTranslation - prevTranslation);
		break;
	}
	case EAFInterpolationType::CubicSpline:
	{
		const float deltaTime = m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex];

		const glm::vec3& prevTangent = deltaTime * glm::vec3(m_channelData.values[prevTimeIndex * 3 + 2]);
		const glm::vec3& nextTangent = deltaTime * glm::vec3(m_channelData.values[nextTimeIndex * 3]);

		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
		const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

		glm::vec3 prevPoint = glm::vec3(m_channelData.values[prevTimeIndex * 3 + 1]);
		glm::vec3 nextPoint = glm::vec3(m_channelData.values[nextTimeIndex * 3 + 1]);

		// @see C++ Game Animation Programming by Dunsky & Szauer
		// About Poses, Frames and Clips.
		finalTranslation = (2 * interpolatedTimeCub -
			3 * interpolatedTimeSq + 1) * prevPoint +
			(interpolatedTimeCub -
				2 * interpolatedTimeSq + interpolatedTime) *
			prevTangent +
			(-2 * interpolatedTimeCub +
				3 * interpolatedTimeSq) * nextPoint +
			(interpolatedTimeCub - interpolatedTimeSq) *
			nextTangent;
		break;
	}
	default:
	{
		break;
	}
	}

	return finalTranslation;
}

glm::quat AFAnimationChannel::GetRotation(float time) const
{
	if (!m_channelData.values)
	{
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	if (time < m_channelData.timings[0])
	{
		return AFMath::Vec4ToQuat(m_channelData.values[0]);
	}

	if (time > m_channelData.timings[m_channelData.keyCount - 1])
	{
		return AFMath::Vec4ToQuat(m_channelData.values[m_channelData.keyCount - 1]);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for (uint32_t i = 0; i < m_channelData.keyCount; ++i)
	{
		if (m_channelData.timings[i] > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if (prevTimeIndex == nextTimeIndex)
	{
		return AFMath::Vec4ToQuat(m_channelData.values[prevTimeIndex]);
	}

	glm::quat finalRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	switch (m_channelData.interpType)
	{
	case EAFInterpolationType::Step:
	{
		finalRotation = AFMath::Vec4ToQuat(m_channelData.values[prevTimeIndex]);
		break;
	}
	case EAFInterpolationType::Linear:
	{
		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const glm::quat& prevRotation = AFMath::Vec4ToQuat(m_channelData.values[prevTimeIndex]);
		const glm::quat& nextRotation = AFMath::Vec4ToQuat(m_channelData.values[nextTimeIndex]);

		finalRotation = glm::slerp(prevRotation, nextRotation, interpolatedTime);
		break;
	}
	case EAFInterpolationType::CubicSpline:
	{
		const float deltaTime = m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex];

		const glm::quat& prevTangent = deltaTime * AFMath::Vec4ToQuat(m_channelData.values[prevTimeIndex * 3 + 2]);
		const glm::quat& nextTangent = deltaTime * AFMath::Vec4ToQuat(m_channelData.values[nextTimeIndex * 3]);

		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
		const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

		glm::quat prevPoint = AFMath::Vec4ToQuat(m_channelData.values[prevTimeIndex * 3 + 1]);
		glm::quat nextPoint = AFMath::Vec4ToQuat(m_channelData.values[nextTimeIndex * 3 + 1]);

		// @see C++ Game Animation Programming by Dunsky & Szauer
		// About Poses, Frames and Clips.
		finalRotation = (2 * interpolatedTimeCub -
			3 * interpolatedTimeSq + 1) * prevPoint +
			(interpolatedTimeCub -
				2 * interpolatedTimeSq + interpolatedTime) *
			prevTangent +
			(-2 * interpolatedTimeCub +
				3 * interpolatedTimeSq) * nextPoint +
			(interpolatedTimeCub - interpolatedTimeSq) *
			nextTangent;
		finalRotation = glm::normalize(finalRotation);
		break;
	}
	default:
	{
		break;
	}
	}

	return finalRotation;
}

glm::vec3 AFAnimationChannel::GetScale(float time) const
{
	if (!m_channelData.values)
	{
		return glm::vec3(0.0f);
	}

	if (time < m_channelData.timings[0])
	{
		return glm::vec3(m_channelData.values[0]);
	}

	if (time > m_channelData.timings[m_channelData.keyCount - 1])
	{
		return glm::vec3(m_channelData.values[m_channelData.keyCount - 1]);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for (uint32_t i = 0; i < m_channelData.keyCount; ++i)
	{
		if (m_channelData.timings[i] > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if (prevTimeIndex == nextTimeIndex)
	{
		return glm::vec3(m_channelData.values[prevTimeIndex]);
	}

	glm::vec3 finalScale = glm::vec3(0.0f);

	switch (m_channelData.interpType)
	{
	case EAFInterpolationType::Step:
	{
		finalScale = glm::vec3(m_channelData.values[prevTimeIndex]);
		break;
	}
	case EAFInterpolationType::Linear:
	{
		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const glm::vec3& prevScale = glm::vec3(m_channelData.values[prevTimeIndex]);
		const glm::vec3& nextScale = glm::vec3(m_channelData.values[nextTimeIndex]);

		finalScale = prevScale + interpolatedTime * (nextScale - prevScale);
		break;
	}
	case EAFInterpolationType::CubicSpline:
	{
		const float deltaTime = m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex];

		const glm::vec3& prevTangent = deltaTime * glm::vec3(m_channelData.values[prevTimeIndex * 3 + 2]);
		const glm::vec3& nextTangent = deltaTime * glm::vec3(m_channelData.values[nextTimeIndex * 3]);

		const float interpolatedTime =
			(time - m_channelData.timings[prevTimeIndex]) /
			(m_channelData.timings[nextTimeIndex] - m_channelData.timings[prevTimeIndex]);

		const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
		const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

		glm::vec3 prevPoint = glm::vec3(m_channelData.values[prevTimeIndex * 3 + 1]);
		glm::vec3 nextPoint = glm::vec3(m_channelData.values[nextTimeIndex * 3 + 1]);

		// @see C++ Game Animation Programming by Dunsky & Szauer
		// About Poses, Frames and Clips.
		finalScale = (2 * interpolatedTimeCub -
			3 * interpolatedTimeSq + 1) * prevPoint +
			(interpolatedTimeCub -
				2 * interpolatedTimeSq + interpolatedTime) *
			prevTangent +
			(-2 * interpolatedTimeCub +
				3 * interpolatedTimeSq) * nextPoint +
			(interpolatedTimeCub - interpolatedTimeSq) *
			nextTangent;
		break;
	}
	default:
	{
		break;
	}
	}

	return finalScale;
}

float AFAnimationChannel::GetMaxTime() const
{
	return m_channelData.timings[m_channelData.keyCount - 1];
}
