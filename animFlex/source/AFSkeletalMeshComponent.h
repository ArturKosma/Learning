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

	const std::vector<glm::mat4>& GetJointsMatrices() const;
	const std::vector<glm::mat4>& GetJointsDualQuatMatrices() const;
	bool GetStateDirty() const;

private:

	std::shared_ptr<AFAnimState> m_animState = nullptr;
};
