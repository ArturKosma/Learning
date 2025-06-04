#include "AFSkeletalMeshComponent.h"
#include "AFAnimState.h"

AFSkeletalMeshComponent::AFSkeletalMeshComponent()
	: AFStaticMeshComponent()
{
	m_animState = std::make_shared<AFAnimState>();
	m_animState->SetOwnerMesh(this);
}

void AFSkeletalMeshComponent::Tick(float deltaTime)
{
	AFStaticMeshComponent::Tick(deltaTime);

	m_animState->Tick(deltaTime);
}

void AFSkeletalMeshComponent::SetAnimation(std::shared_ptr<AFAnimationClip> newAnimation)
{
	m_animState->SetAnimation(newAnimation);
}

void AFSkeletalMeshComponent::AnimationPlay()
{
	m_animState->SetEvaluationState(EAFAnimEvaluationState::Playing);
}

void AFSkeletalMeshComponent::AnimationStop()
{
	m_animState->SetEvaluationState(EAFAnimEvaluationState::Idle);
}

void AFSkeletalMeshComponent::RecalculateSkeleton()
{
	m_mesh->RecalculateSkeleton();
}

glm::vec3 AFSkeletalMeshComponent::GetBoneLocation(int boneIdx) const
{
	return m_mesh->GetJointLocation(boneIdx);
}

glm::quat AFSkeletalMeshComponent::GetBoneRotation(int boneIdx) const
{
	return m_mesh->GetJointRotation(boneIdx);
}

glm::vec3 AFSkeletalMeshComponent::GetBoneScale(int boneIdx) const
{
	return m_mesh->GetJointScale(boneIdx);
}

void AFSkeletalMeshComponent::SetBoneTransform(int boneIdx, const glm::vec3& location, const glm::quat& rotation,
                                               const glm::vec3& scale)
{
	m_mesh->SetJointTransform(boneIdx, location, rotation, scale);
}

const std::vector<glm::mat4>& AFSkeletalMeshComponent::GetJointsMatrices() const
{
	return m_mesh->jointMatrices;
}

const std::vector<glm::mat4>& AFSkeletalMeshComponent::GetJointsDualQuatMatrices() const
{
	return m_mesh->jointDualQuats;
}

bool AFSkeletalMeshComponent::GetStateDirty() const
{
	return m_mesh->jointsDirty;
}
