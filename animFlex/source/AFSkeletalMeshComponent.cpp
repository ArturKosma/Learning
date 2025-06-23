#include "AFSkeletalMeshComponent.h"
#include "AFAnimState.h"
#include "AFMesh.h"

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

std::shared_ptr<AFAnimState> AFSkeletalMeshComponent::GetAnimState() const
{
	return m_animState;
}
