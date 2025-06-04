#pragma once
#include "AFAnimationClip.h"
#include "AFStaticMeshComponent.h"

class AFAnimState;

class AFSkeletalMeshComponent : public AFStaticMeshComponent
{
public:

	AFSkeletalMeshComponent();

	void Tick(float deltaTime) override;

	void SetAnimation(std::shared_ptr<AFAnimationClip> newAnimation);
	void AnimationPlay();
	void AnimationStop();

	void RecalculateSkeleton();

	glm::vec3 GetBoneLocation(int boneIdx) const;
	glm::quat GetBoneRotation(int boneIdx) const;
	glm::vec3 GetBoneScale(int boneIdx) const;
	void SetBoneTransform(int boneIdx, const glm::vec3& location = glm::vec3(0.0f),
		const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f));

	const std::vector<glm::mat4>& GetJointsMatrices() const;
	const std::vector<glm::mat4>& GetJointsDualQuatMatrices() const;
	bool GetStateDirty() const;

private:

	std::shared_ptr<AFAnimState> m_animState = nullptr;
};
