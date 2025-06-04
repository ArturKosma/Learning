#include "AFAnimState.h"
#include "AFSkeletalMeshComponent.h"
#include <chrono>

void AFAnimState::Tick(float deltaTime)
{
	if(m_evaluationState == EAFAnimEvaluationState::Idle)
	{
		return;
	}

	switch(m_sourceState)
	{
		case EAFAnimSourceState::SingleAnim:
		{
			EvaluateSingleAnim();
			break;
		}
		case EAFAnimSourceState::Graph:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void AFAnimState::SetOwnerMesh(AFSkeletalMeshComponent* newOwner)
{
	m_ownerMesh = newOwner;
}

void AFAnimState::SetAnimation(std::shared_ptr<AFAnimationClip> animation)
{
	m_singleAnim = animation;
	m_sourceState = EAFAnimSourceState::SingleAnim;
}

void AFAnimState::SetEvaluationState(EAFAnimEvaluationState newEvaluationState)
{
	m_evaluationState = newEvaluationState;
}

void AFAnimState::EvaluateSingleAnim()
{
	if (!(m_ownerMesh && m_singleAnim))
	{
		return;
	}

	const long long currentTime = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::steady_clock::now().time_since_epoch()
	).count();
	const float time = std::fmod(static_cast<float>(currentTime) / 1000.0f, m_singleAnim->GetClipEndTime());

	const std::vector<std::shared_ptr<AFNode>>& joints = m_ownerMesh->GetMesh()->GetJoints();
	const std::vector<int>& nodes = m_ownerMesh->GetMesh()->GetNodeToJoint();

	for (auto& channel : m_singleAnim->GetAnimationChannels())
	{
		int targetJoint = nodes[channel->GetTargetNode()] - 1;
		if(targetJoint < 0)
		{
			continue;
		}

		switch (channel->GetTargetPath())
		{
		case EAFTargetPath::Translation:
		{
			joints.at(targetJoint)->SetLocation(channel->GetTranslation(time));
			break;
		}
		case EAFTargetPath::Rotation:
		{
			joints.at(targetJoint)->SetRotation(channel->GetRotation(time));
			break;
		}
		case EAFTargetPath::Scale:
		{
			joints.at(targetJoint)->SetScale(channel->GetScale(time));
			break;
		}
		default:
		{
			break;
		}
		}
	}

	for (auto& joint : joints)
	{
		joint->CalculateLocalTRSMatrix();
	}

	m_ownerMesh->GetMesh()->jointsDirty = true;
}
