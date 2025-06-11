#include "AFAnimationClip.h"
#include "AFGLTFLoader.h"
#include <fstream>
#include <sstream>
#include "zstd.h"

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

bool AFAnimationClip::LoadImpl(const char* filepath)
{
	if (AFGLTFLoader::LoadAnim(filepath, this))
	{
		return true;
	}

	return false;
}

bool AFAnimationClip::SerializeImpl(const char* filepath)
{
	// Open out stream.
	std::ostringstream outstream(std::ios_base::out);

	// Write animation's string name size.
	const uint16_t nameStringSize = static_cast<uint16_t>(m_clipName.size());
	outstream.write(reinterpret_cast<const char*>(&nameStringSize), sizeof(uint16_t));

	// Write animation's name.
	outstream.write(m_clipName.data(), nameStringSize);

	// Write key-count.
	const FAFAnimationChannelData& anyChannel = m_animationChannels[0]->GetChannelData();
	const uint32_t keycount = anyChannel.keyCount;
	outstream.write(reinterpret_cast<const char*>(&keycount), sizeof(uint32_t));

	// Write channel-count.
	const uint32_t channelCount = static_cast<uint32_t>(m_animationChannels.size());
	outstream.write(reinterpret_cast<const char*>(&channelCount), sizeof(uint32_t));

	// For each channel.
	for(uint32_t i = 0; i < channelCount; ++i)
	{
		// Write target joint index.
		outstream.write(reinterpret_cast<const char*>(&m_animationChannels[i]->GetChannelData().targetJoint), sizeof(uint16_t));

		// Write target path.
		outstream.write(reinterpret_cast<const char*>(&m_animationChannels[i]->GetChannelData().targetPath), sizeof(uint8_t));

		// Write interp type.
		outstream.write(reinterpret_cast<const char*>(&m_animationChannels[i]->GetChannelData().interpType), sizeof(uint8_t));

		// Write timings.
		outstream.write(reinterpret_cast<const char*>(m_animationChannels[i]->GetChannelData().timings), static_cast<uint64_t>(sizeof(float) * keycount));

		// Write values.
		outstream.write(reinterpret_cast<const char*>(m_animationChannels[i]->GetChannelData().values), static_cast<uint64_t>(sizeof(glm::vec4) * keycount));
	}

	// Get raw buffer and max compression size.
	std::string rawBuffer = outstream.str();
	uint64_t rawBufferSize = rawBuffer.size();

	// Compress.
	uint64_t maxCompressedSize = static_cast<int>(ZSTD_compressBound(rawBuffer.size()));
	std::vector<char> compressedBuffer(maxCompressedSize);
	uint64_t compressedSize = ZSTD_compress(compressedBuffer.data(), maxCompressedSize, rawBuffer.data(), rawBufferSize, 3);

	// Open file.
	std::ofstream outfile(filepath, std::ofstream::binary);

	// Write compressed and raw sizes.
	outfile.write(reinterpret_cast<const char*>(&compressedSize), sizeof(uint64_t));
	outfile.write(reinterpret_cast<const char*>(&rawBufferSize), sizeof(uint64_t));

	// Write compressed data.
	outfile.write(compressedBuffer.data(), compressedSize);

	outfile.close();
	
	return false;
}

bool AFAnimationClip::DeserializeImpl(const char* filepath)
{
	// Open file.
	std::ifstream infile(filepath, std::ifstream::binary);

	// Read compressed and original sizes.
	uint64_t compressedSize = 0;
	uint64_t rawSize = 0;
	infile.read(reinterpret_cast<char*>(&compressedSize), sizeof(uint64_t));
	infile.read(reinterpret_cast<char*>(&rawSize), sizeof(uint64_t));

	// Read compressed data.
	std::vector<char> compressedData(compressedSize);
	infile.read(compressedData.data(), compressedSize);

	infile.close();

	// Decompress.
	std::vector<char> decompressedData(rawSize);
	ZSTD_decompress(decompressedData.data(), rawSize, compressedData.data(), compressedSize);

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

