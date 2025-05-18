#pragma once
#include <memory>

#include "AFStructs.h"
#include "third_party/tiny_gltf.h"

class AFAnimationChannel
{
public:

	void LoadChannelData(std::shared_ptr<tinygltf::Model> model, tinygltf::Animation anim, tinygltf::AnimationChannel channel);

	int GetTargetNode() const;
	EAFTargetPath GetTargetPath() const;

	glm::vec3 GetTranslation(float time) const;
	glm::quat GetRotation(float time) const;
	glm::vec3 GetScale(float time) const;

	float GetMaxTime();

private:

	int m_targetNode = -1;
	EAFTargetPath m_targetPath = EAFTargetPath::Rotation;
	EAFInterpolationType m_interpType = EAFInterpolationType::Linear;

	std::vector<float> m_timings = {};
	std::vector<glm::vec3> m_translations = {};
	std::vector<glm::quat> m_rotations = {};
	std::vector<glm::vec3> m_scales = {};

	void SetTimings(const std::vector<float>& timings);
	void SetTranslations(const std::vector<glm::vec3>& translations);
	void SetRotations(const std::vector<glm::quat>& rotations);
	void SetScales(const std::vector<glm::vec3>& scales);
};
