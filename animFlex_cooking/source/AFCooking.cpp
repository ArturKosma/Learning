#include "AFCooking.h"
#include "third_party/tiny_gltf.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

#include "json.hpp"
#include "zstd.h"

std::string AFCooking::CookFile(const std::string& type, const std::string& sourcePath, const std::string& targetPath, const std::string& additionalArgs)
{
	if (type == "anim")
	{
		return CookAnim(sourcePath, targetPath);
	}
	else if (type == "animCurve")
	{
		return CookAnimCurve(sourcePath, targetPath, additionalArgs);
	}

	return "";
}

std::string AFCooking::CookAnim(const std::string& sourcePath, const std::string& targetPath)
{
	// Load via TinyGLTF.
	std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = loader.LoadBinaryFromFile(&*model, &err, &warn, sourcePath);
	if (!ret)
	{
		ret = loader.LoadASCIIFromFile(&*model, &err, &warn, sourcePath);
	}

	if (!ret)
	{
		printf("Failed to parse glTF: %s\n", sourcePath.c_str());
		return "";
	}

	if (model->animations.empty())
	{
		printf("No animations in file.\n");
		return "";
	}

	// Save just the filename, without extension nor directory.
	std::filesystem::path p(sourcePath);
	const std::string& animName = p.filename().stem().string();

	// Prepare space for anim channels.
	std::vector<FAFAnimationChannelData> channels;

	// We assume to have only 1 animation per file.
	for (const auto& anim : model->animations)
	{
		for (const auto& channel : anim.channels)
		{
			FAFAnimationChannelData newChannel;

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
			newChannel.keyCount = keyCount;

			// Get joint for this channel.
			newChannel.targetJoint = static_cast<uint16_t>(channel.target_node);

			// Allocate enough memory for timings and values arrays.
			newChannel.timings = static_cast<float*>(std::malloc(sizeof(float) * keyCount));
			newChannel.values = static_cast<glm::vec4*>(std::malloc(sizeof(glm::vec4) * keyCount));

			// Get timings for every frame.
			std::memcpy(newChannel.timings, &inputBuffer.data.at(0) + inputBufferView.byteOffset, inputBufferView.byteLength);

			// Get interpolation type between frames.
			const tinygltf::AnimationSampler sampler = anim.samplers.at(channel.sampler);
			if (sampler.interpolation == "STEP")
			{
				newChannel.interpType = EAFInterpolationType::Step;
			}
			else if (sampler.interpolation == "LINEAR")
			{
				newChannel.interpType = EAFInterpolationType::Linear;
			}
			else if (sampler.interpolation == "CUBICSPLINE")
			{
				newChannel.interpType = EAFInterpolationType::CubicSpline;
			}

			// Get target path for this channel.
			if (channel.target_path == "translation")
			{
				newChannel.targetPath = EAFTargetPath::Translation;
			}
			else if (channel.target_path == "rotation")
			{
				newChannel.targetPath = EAFTargetPath::Rotation;
			}
			else if (channel.target_path == "scale")
			{
				newChannel.targetPath = EAFTargetPath::Scale;
			}

			// Get values for this channel.
			const uint8_t* rawValues = &outputBuffer.data.at(0) + outputBufferView.byteOffset;
			if (outputAccessor.type == TINYGLTF_TYPE_VEC3)
			{
				const glm::vec3* src = reinterpret_cast<const glm::vec3*>(rawValues);
				for (size_t i = 0; i < outputAccessor.count; ++i)
				{
					newChannel.values[i] = glm::vec4(src[i], 0.0f); // Copy with padding w = 0.
				}
			}
			else if (outputAccessor.type == TINYGLTF_TYPE_VEC4)
			{
				const glm::vec4* src = reinterpret_cast<const glm::vec4*>(rawValues);
				std::memcpy(newChannel.values, src, sizeof(glm::vec4) * outputAccessor.count);
			}

			channels.push_back(newChannel);
		}
	}

	// Open out stream.
	std::ostringstream outstream(std::ios_base::out);

	// Write animation's string name size.
	const uint16_t nameStringSize = static_cast<uint16_t>(animName.size());
	outstream.write(reinterpret_cast<const char*>(&nameStringSize), sizeof(uint16_t));

	// Write animation's name.
	outstream.write(animName.data(), nameStringSize);

	// Write key-count.
	const FAFAnimationChannelData& anyChannel = channels[0];
	const uint32_t keycount = anyChannel.keyCount;
	outstream.write(reinterpret_cast<const char*>(&keycount), sizeof(uint32_t));

	// Write channel-count.
	const uint32_t channelCount = static_cast<uint32_t>(channels.size());
	outstream.write(reinterpret_cast<const char*>(&channelCount), sizeof(uint32_t));

	// For each channel.
	for (uint32_t i = 0; i < channelCount; ++i)
	{
		// Write target joint index.
		outstream.write(reinterpret_cast<const char*>(&channels[i].targetJoint), sizeof(uint16_t));

		// Write target path.
		outstream.write(reinterpret_cast<const char*>(&channels[i].targetPath), sizeof(uint8_t));

		// Write interp type.
		outstream.write(reinterpret_cast<const char*>(&channels[i].interpType), sizeof(uint8_t));

		// Write timings.
		outstream.write(reinterpret_cast<const char*>(channels[i].timings), static_cast<uint64_t>(sizeof(float) * keycount));

		// Write values.
		outstream.write(reinterpret_cast<const char*>(channels[i].values), static_cast<uint64_t>(sizeof(glm::vec4) * keycount));
	}

	// Get raw buffer and max compression size.
	std::string rawBuffer = outstream.str();
	uint64_t rawBufferSize = rawBuffer.size();

	// Compress.
	uint64_t maxCompressedSize = static_cast<int>(ZSTD_compressBound(rawBuffer.size()));
	std::vector<char> compressedBuffer(maxCompressedSize);
	uint64_t compressedSize = ZSTD_compress(compressedBuffer.data(), maxCompressedSize, rawBuffer.data(), rawBufferSize, 3);

	std::string fullOutputPath = targetPath + "\\" + animName + ".afanim";

	// Open file.
	std::ofstream outfile(fullOutputPath, std::ofstream::binary);

	// Write compressed and raw sizes.
	outfile.write(reinterpret_cast<const char*>(&compressedSize), sizeof(uint64_t));
	outfile.write(reinterpret_cast<const char*>(&rawBufferSize), sizeof(uint64_t));

	// Write compressed data.
	outfile.write(compressedBuffer.data(), compressedSize);

	printf("cooked: %s -> %s\n", animName.c_str(), fullOutputPath.c_str());

	outfile.close();

	return animName;
}

std::string AFCooking::CookAnimCurve(const std::string& sourcePath, const std::string& targetPath,
	const std::string& additionalArgs)
{
	// Read additional args.
	const std::vector<std::string> args = AFCooking::ReadAdditionalArgs(additionalArgs);

	std::string requestedMotionType = "";
	int requestedBoneIdx = 0;
	std::string requestedAxis = "";

	// Figure out what the user wants based on args.
	for (const std::string& arg : args)
	{
		if (arg == "rootDistance")
		{
			requestedMotionType = "rootDistance";
			requestedBoneIdx = 0;
		}
	}

	// Load via TinyGLTF.
	std::shared_ptr<tinygltf::Model> model = std::make_shared<tinygltf::Model>();
	tinygltf::TinyGLTF loader;
	std::string err;
	std::string warn;

	bool ret = loader.LoadBinaryFromFile(&*model, &err, &warn, sourcePath);
	if (!ret)
	{
		ret = loader.LoadASCIIFromFile(&*model, &err, &warn, sourcePath);
	}

	if (!ret)
	{
		printf("Failed to parse glTF: %s\n", sourcePath.c_str());
		return "";
	}

	if (model->animations.empty())
	{
		printf("No animations in file.\n");
		return "";
	}

	// Save just the filename, without extension nor directory.
	std::filesystem::path p(sourcePath);
	const std::string& animName = p.filename().stem().string();
	std::string curveName = "";

	// We assume to have only 1 animation per file.
	for (const auto& anim : model->animations)
	{
		for (const auto& channel : anim.channels)
		{
			FAFAnimationChannelData newChannel;

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
			newChannel.keyCount = keyCount;

			// Get joint for this channel.
			newChannel.targetJoint = static_cast<uint16_t>(channel.target_node);

			if (newChannel.targetJoint != requestedBoneIdx)
			{
				continue;
			}

			// Allocate enough memory for timings and values arrays.
			newChannel.timings = static_cast<float*>(std::malloc(sizeof(float) * keyCount));
			newChannel.values = static_cast<glm::vec4*>(std::malloc(sizeof(glm::vec4) * keyCount));

			// Get timings for every frame.
			std::memcpy(newChannel.timings, &inputBuffer.data.at(0) + inputBufferView.byteOffset, inputBufferView.byteLength);

			// Get interpolation type between frames.
			const tinygltf::AnimationSampler sampler = anim.samplers.at(channel.sampler);
			if (sampler.interpolation == "STEP")
			{
				newChannel.interpType = EAFInterpolationType::Step;
			}
			else if (sampler.interpolation == "LINEAR")
			{
				newChannel.interpType = EAFInterpolationType::Linear;
			}
			else if (sampler.interpolation == "CUBICSPLINE")
			{
				newChannel.interpType = EAFInterpolationType::CubicSpline;
			}

			// Get target path for this channel.
			if (channel.target_path == "translation")
			{
				newChannel.targetPath = EAFTargetPath::Translation;
			}
			else if (channel.target_path == "rotation")
			{
				newChannel.targetPath = EAFTargetPath::Rotation;
			}
			else if (channel.target_path == "scale")
			{
				newChannel.targetPath = EAFTargetPath::Scale;
			}

			// Get values for this channel.
			const uint8_t* rawValues = &outputBuffer.data.at(0) + outputBufferView.byteOffset;
			if (outputAccessor.type == TINYGLTF_TYPE_VEC3)
			{
				const glm::vec3* src = reinterpret_cast<const glm::vec3*>(rawValues);
				for (size_t i = 0; i < outputAccessor.count; ++i)
				{
					newChannel.values[i] = glm::vec4(src[i], 0.0f); // Copy with padding w = 0.
				}
			}
			else if (outputAccessor.type == TINYGLTF_TYPE_VEC4)
			{
				const glm::vec4* src = reinterpret_cast<const glm::vec4*>(rawValues);
				std::memcpy(newChannel.values, src, sizeof(glm::vec4) * outputAccessor.count);
			}

			// Root Distance - reads the length of vector of the root bone.
			if (requestedMotionType == "rootDistance")
			{
				nlohmann::json rootDistanceArray = nlohmann::json::array();

				// We are only interested in the translation.
				if (newChannel.targetPath != EAFTargetPath::Translation)
				{
					continue;
				}

				// Get the vector at every key.
				for (uint32_t i = 0; i < keyCount; ++i)
				{
					const float valueX = newChannel.values[i].x;
					const float valueY = newChannel.values[i].y;
					const float valueZ = newChannel.values[i].z;

					const float distance = glm::length(glm::vec3(valueX, valueY, valueZ));

					rootDistanceArray.push_back({ newChannel.timings[i], distance });
				}

				std::string filename = "";
				filename += animName;
				filename += "_rootDistance";
				curveName = filename;
				filename += ".json";
				const std::string& curvePath = (std::filesystem::path(targetPath) / filename).string();
				std::ofstream outJson(curvePath);
				outJson << rootDistanceArray.dump(2);

				printf("cooked curve: %s -> %s\n", filename.c_str(), curvePath.c_str());
			}
		}
	}

	return curveName;
}

std::vector<std::string> AFCooking::ReadAdditionalArgs(const std::string& additionalArgs)
{
	std::vector<std::string> ret = {};
	std::string currentArg = "";

	for (char c : additionalArgs)
	{
		if (c == '-')
		{
			if (!currentArg.empty())
			{
				ret.push_back(currentArg);
			}

			currentArg.clear();
		}
		else
		{
			currentArg += c;
		}
	}

	if (!currentArg.empty())
	{
		ret.push_back(currentArg);
	}

	return ret;
}
