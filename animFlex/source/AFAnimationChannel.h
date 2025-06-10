#pragma once
#include <memory>

#include "AFStructs.h"
#include "third_party/tiny_gltf.h"

struct FAFAnimationChannelData
{
	uint16_t targetJoint = 0;
	EAFTargetPath targetPath = EAFTargetPath::Rotation;
	EAFInterpolationType interpType = EAFInterpolationType::Linear;
	uint32_t keyCount = 0;

	float* timings = nullptr;
	glm::vec4* values = nullptr;
};

class AFAnimationChannel
{
public:

	~AFAnimationChannel();

	void LoadChannelData(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim, tinygltf::AnimationChannel channel);

	int GetTargetJoint() const;
	EAFTargetPath GetTargetPath() const;

	glm::vec3 GetTranslation(float time) const;
	glm::quat GetRotation(float time) const;
	glm::vec3 GetScale(float time) const;

	float GetMaxTime() const;

private:

	FAFAnimationChannelData m_channelData = {};
};
