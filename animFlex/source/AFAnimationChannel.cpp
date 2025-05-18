#include "AFAnimationChannel.h"

void AFAnimationChannel::LoadChannelData(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim,
	tinygltf::AnimationChannel channel)
{
	m_targetNode = channel.target_node;

	const tinygltf::Accessor& inputAccessor = model->accessors.at(anim.samplers.at(channel.sampler).input);
	const tinygltf::BufferView& inputBufferView = model->bufferViews.at(inputAccessor.bufferView);
	const tinygltf::Buffer& inputBuffer = model->buffers.at(inputBufferView.buffer);

	std::vector<float> timings;
	timings.resize(inputAccessor.count);
	std::memcpy(timings.data(), &inputBuffer.data.at(0) + inputBufferView.byteOffset, inputBufferView.byteLength);
	SetTimings(timings);

	const tinygltf::AnimationSampler sampler = anim.samplers.at(channel.sampler);
	if(sampler.interpolation == "STEP")
	{
		m_interpType = EAFInterpolationType::Step;
	}
	else if(sampler.interpolation == "LINEAR")
	{
		m_interpType = EAFInterpolationType::Linear;
	}
	else if (sampler.interpolation == "CUBICSPLINE")
	{
		m_interpType = EAFInterpolationType::CubicSpline;
	}

	const tinygltf::Accessor& outputAccessor = model->accessors.at(anim.samplers.at(channel.sampler).output);
	const tinygltf::BufferView& outputBufferView = model->bufferViews.at(outputAccessor.bufferView);
	const tinygltf::Buffer& outputBuffer = model->buffers.at(outputBufferView.buffer);

	if(channel.target_path == "translation")
	{
		m_targetPath = EAFTargetPath::Translation;
		std::vector<glm::vec3> translations;
		translations.resize(outputAccessor.count);
		std::memcpy(translations.data(), &outputBuffer.data.at(0) + outputBufferView.byteOffset, outputBufferView.byteLength);
		SetTranslations(translations);
	}
	else if (channel.target_path == "rotation")
	{
		m_targetPath = EAFTargetPath::Rotation;
		std::vector<glm::quat> rotations;
		rotations.resize(outputAccessor.count);
		std::memcpy(rotations.data(), &outputBuffer.data.at(0) + outputBufferView.byteOffset, outputBufferView.byteLength);
		SetRotations(rotations);
	}
	else if (channel.target_path == "scale")
	{
		m_targetPath = EAFTargetPath::Scale;
		std::vector<glm::vec3> scales;
		scales.resize(outputAccessor.count);
		std::memcpy(scales.data(), &outputBuffer.data.at(0) + outputBufferView.byteOffset, outputBufferView.byteLength);
		SetScales(scales);
	}
}

int AFAnimationChannel::GetTargetNode() const
{
	return m_targetNode;
}

EAFTargetPath AFAnimationChannel::GetTargetPath() const
{
	return m_targetPath;
}

glm::vec3 AFAnimationChannel::GetTranslation(float time) const
{
	if (m_translations.empty())
	{
		return glm::vec3(0.0f);
	}

	if (time < m_timings.at(0))
	{
		return m_translations.at(0);
	}

	if (time > m_timings.at(m_timings.size() - 1))
	{
		return m_translations.at(m_translations.size() - 1);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for (int i = 0; i < m_timings.size(); ++i)
	{
		if (m_timings.at(i) > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if (prevTimeIndex == nextTimeIndex)
	{
		return m_translations.at(prevTimeIndex);
	}

	glm::vec3 finalTranslation = glm::vec3(0.0f);

	switch (m_interpType)
	{
	case EAFInterpolationType::Step:
	{
		finalTranslation = m_translations.at(prevTimeIndex);
		break;
	}
	case EAFInterpolationType::Linear:
	{
		const float interpolatedTime =
			(time - m_timings.at(prevTimeIndex)) /
			(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

		const glm::vec3& prevScale = m_translations.at(prevTimeIndex);
		const glm::vec3& nextScale = m_translations.at(nextTimeIndex);

		finalTranslation = prevScale + interpolatedTime * (nextScale - prevScale);
		break;
	}
	case EAFInterpolationType::CubicSpline:
	{
		const float deltaTime = m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex);

		const glm::vec3& prevTangent = deltaTime * m_translations.at(prevTimeIndex * 3 + 2);
		const glm::vec3& nextTangent = deltaTime * m_translations.at(nextTimeIndex * 3);

		const float interpolatedTime =
			(time - m_timings.at(prevTimeIndex)) /
			(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

		const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
		const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

		glm::vec3 prevPoint = m_translations.at(prevTimeIndex * 3 + 1);
		glm::vec3 nextPoint = m_translations.at(nextTimeIndex * 3 + 1);

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
	if (m_rotations.empty())
	{
		return { 1.0f, 0.0f, 0.0f, 0.0f };
	}

	if (time < m_timings.at(0))
	{
		return m_rotations.at(0);
	}

	if (time > m_timings.at(m_timings.size() - 1))
	{
		return m_rotations.at(m_rotations.size() - 1);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for (int i = 0; i < m_timings.size(); ++i)
	{
		if (m_timings.at(i) > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if (prevTimeIndex == nextTimeIndex)
	{
		return m_rotations.at(prevTimeIndex);
	}

	glm::quat finalRotation = {1.0f, 0.0f, 0.0f, 0.0f};

	switch (m_interpType)
	{
	case EAFInterpolationType::Step:
	{
		finalRotation = m_rotations.at(prevTimeIndex);
		break;
	}
	case EAFInterpolationType::Linear:
	{
		const float interpolatedTime =
			(time - m_timings.at(prevTimeIndex)) /
			(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

		const glm::quat& prevScale = m_rotations.at(prevTimeIndex);
		const glm::quat& nextScale = m_rotations.at(nextTimeIndex);

		finalRotation = prevScale + interpolatedTime * (nextScale - prevScale);
		break;
	}
	case EAFInterpolationType::CubicSpline:
	{
		const float deltaTime = m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex);

		const glm::quat& prevTangent = deltaTime * m_rotations.at(prevTimeIndex * 3 + 2);
		const glm::quat& nextTangent = deltaTime * m_rotations.at(nextTimeIndex * 3);

		const float interpolatedTime =
			(time - m_timings.at(prevTimeIndex)) /
			(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

		const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
		const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

		glm::quat prevPoint = m_rotations.at(prevTimeIndex * 3 + 1);
		glm::quat nextPoint = m_rotations.at(nextTimeIndex * 3 + 1);

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
	if(m_scales.empty())
	{
		return glm::vec3(1.0f);
	}

	if(time < m_timings.at(0))
	{
		return m_scales.at(0);
	}

	if(time > m_timings.at(m_timings.size() - 1))
	{
		return m_scales.at(m_scales.size() - 1);
	}

	int prevTimeIndex = 0;
	int nextTimeIndex = 0;
	for(int i = 0; i < m_timings.size(); ++i)
	{
		if(m_timings.at(i) > time)
		{
			nextTimeIndex = i;
			break;
		}
		prevTimeIndex = i;
	}

	if(prevTimeIndex == nextTimeIndex)
	{
		return m_scales.at(prevTimeIndex);
	}

	glm::vec3 finalScale = glm::vec3(1.0f);

	switch(m_interpType)
	{
		case EAFInterpolationType::Step:
		{
			finalScale = m_scales.at(prevTimeIndex);
			break;
		}
		case EAFInterpolationType::Linear:
		{
			const float interpolatedTime =
				(time - m_timings.at(prevTimeIndex)) /
				(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

			const glm::vec3& prevScale = m_scales.at(prevTimeIndex);
			const glm::vec3& nextScale = m_scales.at(nextTimeIndex);

			finalScale = prevScale + interpolatedTime * (nextScale - prevScale);
			break;
		}
		case EAFInterpolationType::CubicSpline:
		{
			const float deltaTime = m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex);

			const glm::vec3& prevTangent = deltaTime * m_scales.at(prevTimeIndex * 3 + 2);
			const glm::vec3& nextTangent = deltaTime * m_scales.at(nextTimeIndex * 3);

			const float interpolatedTime =
				(time - m_timings.at(prevTimeIndex)) /
				(m_timings.at(nextTimeIndex) - m_timings.at(prevTimeIndex));

			const float interpolatedTimeSq = interpolatedTime * interpolatedTime;
			const float interpolatedTimeCub = interpolatedTimeSq * interpolatedTime;

			glm::vec3 prevPoint = m_scales.at(prevTimeIndex * 3 + 1);
			glm::vec3 nextPoint = m_scales.at(nextTimeIndex * 3 + 1);

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

float AFAnimationChannel::GetMaxTime()
{
	return m_timings.at(m_timings.size() - 1);
}

void AFAnimationChannel::SetTimings(const std::vector<float>& timings)
{
	m_timings = timings;
}

void AFAnimationChannel::SetTranslations(const std::vector<glm::vec3>& translations)
{
	m_translations = translations;
}

void AFAnimationChannel::SetRotations(const std::vector<glm::quat>& rotations)
{
	m_rotations = rotations;
}

void AFAnimationChannel::SetScales(const std::vector<glm::vec3>& scales)
{
	m_scales = scales;
}
