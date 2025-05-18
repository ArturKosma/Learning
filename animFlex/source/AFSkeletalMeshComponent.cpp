#include "AFSkeletalMeshComponent.h"

#include <chrono>

void AFSkeletalMeshComponent::SetAnimation(std::shared_ptr<AFAnimationClip> newAnimation)
{
	m_animation = newAnimation;
}

void AFSkeletalMeshComponent::PlayAnimation(float playrate)
{
	const double currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch()
	).count();

	SetAnimationFrame(std::fmod(currentTime / 1000.0 * playrate, m_animation->GetClipEndTime()));
}

void AFSkeletalMeshComponent::SetAnimationFrame(float time)
{
	m_animation->SetAnimationFrame(m_mesh->GetJoints(), time);
	m_mesh->jointsDirty = true;
}

std::shared_ptr<AFAnimationClip> AFSkeletalMeshComponent::GetAnimation() const
{
	return m_animation;
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
