#include "AFAnimState.h"
#include "AFSkeletalMeshComponent.h"
#include <chrono>

#include "AFEvaluator.h"
#include "AFMesh.h"

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
			EvaluateGraph(deltaTime);
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

void AFAnimState::SetGraph(std::shared_ptr<AFAnimGraph> animGraph)
{
	m_graph = animGraph;
	m_sourceState = EAFAnimSourceState::Graph;
}

std::shared_ptr<AFAnimGraph> AFAnimState::GetGraph() const
{
	return m_graph;
}

float AFAnimState::GetCurveValue(const std::string& curveName) const
{
	auto it = m_curves.find(curveName);
	if (it != m_curves.end())
	{
		return it->second;
	}

	return 0.0f;
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

	const std::vector<std::shared_ptr<AFJoint>>& joints = m_ownerMesh->GetMesh()->GetJoints();

	for (auto& channel : m_singleAnim->GetAnimationChannels())
	{
		const int targetJoint = channel->GetTargetJoint();

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

void AFAnimState::EvaluateGraph(float deltaTime)
{
	if (!m_graph || !m_ownerMesh)
	{
		return;
	}

	// Mark all nodes dirty. This will allow for new evaluation of each of them.
	AFEvaluator::Get().ClearEvaluationState();

	AFEvaluator::Get().ClearLastActiveSockets();
	AFEvaluator::Get().ClearLastActiveStates();
	AFEvaluator::Get().ClearSamplingState();

	m_graph->Evaluate(deltaTime);

	const std::vector<std::shared_ptr<AFJoint>>& calculatedJoints = m_graph->GetFinalPose().GetJoints();
	const std::vector<std::shared_ptr<AFJoint>>& currentJoints = m_ownerMesh->GetMesh()->GetJoints();

	// Copy curves without resetting.
	std::unordered_map<std::string, float> curves = m_graph->GetFinalPose().GetCurvesValues();
	for (const auto& [name, value] : curves)
	{
		m_curves[name] = value;
	}

	//printf("%f\n", GetCurveValue("rootYawDelta"));

	if (calculatedJoints.size() != currentJoints.size())
	{
		return;
	}

	for (int i = 0; i < currentJoints.size(); ++i)
	{
		currentJoints[i]->SetLocation(calculatedJoints[i]->GetLocation());
		currentJoints[i]->SetRotation(calculatedJoints[i]->GetRotation());
		currentJoints[i]->SetScale(calculatedJoints[i]->GetScale());

		currentJoints[i]->CalculateLocalTRSMatrix();
	}

	m_ownerMesh->GetMesh()->jointsDirty = true;
}
