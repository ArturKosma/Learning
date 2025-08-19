#include "AFAnimationClip.h"
#include "AFGLTFLoader.h"
#include <fstream>
#include <sstream>

#include "AFContent.h"
#include "zstd.h"

void AFAnimationClip::OnLoadComplete()
{
	// Add curves for this clip.
	// We assume curves to be loaded beforehand, and their name contains full anim clip name.
	// Usually (Unreal Engine 5) curves are a part of Animation Sequence.
	// Hack.
	const std::vector<std::shared_ptr<AFFloatCurve>> curves = AFContent::Get().FindAssets<AFFloatCurve>(GetClipName().c_str());
	for (std::shared_ptr<AFFloatCurve> curve : curves)
	{
		AddCurve(curve->GetName(), curve);
	}
}

void AFAnimationClip::AddChannel(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim,
                                 tinygltf::AnimationChannel channel)
{
	std::shared_ptr<AFAnimationChannel> chan = std::make_shared<AFAnimationChannel>();
	chan->LoadChannelData(model, anim, channel);

	m_animationChannels.push_back(chan);
}

const std::vector<std::shared_ptr<AFAnimationChannel>>& AFAnimationClip::GetAnimationChannels() const
{
	return m_animationChannels;
}

float AFAnimationClip::GetClipEndTime() const
{
	return m_animationChannels.at(0)->GetMaxTime();
}

void AFAnimationClip::SetClipName(const std::string& newName)
{
	m_clipName = newName;
}

std::string AFAnimationClip::GetClipName() const
{
	return m_clipName;
}

void AFAnimationClip::AddCurve(const std::string& curveName, std::shared_ptr<AFFloatCurve> newCurve)
{
	m_curves.insert({ curveName, newCurve });
}

std::shared_ptr<AFFloatCurve> AFAnimationClip::GetCurve(const std::string& curveName)
{
	return m_curves[curveName];
}

bool AFAnimationClip::LoadImpl(const char* filepath)
{
	if (AFGLTFLoader::LoadAnim(filepath, this))
	{
		return true;
	}

	return false;
}

bool AFAnimationClip::DeserializeImpl(const char* stream)
{
	// Cast the stream to a byte pointer.
	char* cursor = const_cast<char*>(stream);

	// Read compressed size.
	uint64_t compressedSize = *reinterpret_cast<uint64_t*>(cursor);
	cursor += sizeof(uint64_t);

	// Read raw size.
	uint64_t rawSize = *reinterpret_cast<uint64_t*>(cursor);
	cursor += sizeof(uint64_t);

	// Read compressed data directly from memory.
	const char* compressedData = cursor;

	// Decompress.
	std::vector<char> decompressedData(rawSize);
	ZSTD_decompress(decompressedData.data(), rawSize, compressedData, compressedSize);

	// Create a buffer for deserialization.
	std::istringstream rawStream(std::string(decompressedData.begin(), decompressedData.end()), std::ios::binary);

	// Read animation's name string size.
	uint16_t nameStringSize = 0;
	rawStream.read(reinterpret_cast<char*>(&nameStringSize), sizeof(uint16_t));

	// Read animation's name string.
	std::string name(nameStringSize, '\0');
	rawStream.read(name.data(), nameStringSize);

	// Save the clip name.
	m_clipName = name;

	// Read key-count.
	uint32_t keyCount = 0;
	rawStream.read(reinterpret_cast<char*>(&keyCount), sizeof(uint32_t));

	// Read channel-count.
	uint32_t channelCount = 0;
	rawStream.read(reinterpret_cast<char*>(&channelCount), sizeof(uint32_t));

	// For each channel.
	for(uint32_t i = 0; i < channelCount; ++i)
	{
		// Read target joint index.
		uint16_t jointIndex = 0;
		rawStream.read(reinterpret_cast<char*>(&jointIndex), sizeof(uint16_t));

		// Read target path.
		uint8_t targetPath = 0;
		rawStream.read(reinterpret_cast<char*>(&targetPath), sizeof(uint8_t));

		// Read interp path.
		uint8_t interpType = 0;
		rawStream.read(reinterpret_cast<char*>(&interpType), sizeof(uint8_t));

		// Read timings.
		float* timings = static_cast<float*>(malloc(sizeof(float) * keyCount));
		rawStream.read(reinterpret_cast<char*>(timings), static_cast<uint64_t>(sizeof(float) * keyCount));

		// Read values.
		glm::vec4* values = static_cast<glm::vec4*>(malloc(sizeof(glm::vec4) * keyCount));
		rawStream.read(reinterpret_cast<char*>(values), static_cast<uint64_t>(sizeof(glm::vec4) * keyCount));

		// New channel creation.
		std::shared_ptr<AFAnimationChannel> newChannel = std::make_shared<AFAnimationChannel>();
		FAFAnimationChannelData channelData;

		// Fill new channel.
		channelData.targetJoint = jointIndex;
		channelData.targetPath = static_cast<EAFTargetPath>(targetPath);
		channelData.interpType = static_cast<EAFInterpolationType>(interpType);
		channelData.keyCount = keyCount;
		channelData.timings = timings;
		channelData.values = values;
		newChannel->LoadChannelData(channelData);

		// Add new channel to the clip.
		m_animationChannels.push_back(newChannel);
	}

	return false;
}

