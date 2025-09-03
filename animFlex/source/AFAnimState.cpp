#include "AFAnimState.h"
#include "AFSkeletalMeshComponent.h"
#include <chrono>

#include "AFCharacterMovementComponent.h"
#include "AFEvaluator.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"
#include "AFUtility.h"

void AFAnimState::PreTick(float deltaTime)
{
	std::shared_ptr<AFPlayerPawn> pawn = AFGame::GetGame()->GetScene().GetPlayerPawn();
	if (!pawn)
	{
		return;
	}

	// Counter the control yaw (actor rotates with control yaw, root bone counters it).
	// I do this on PreTick, because PreTick is checking entry states, like StartRun, which might calculate angle needed before reaching Tick.
	// PreTick happens after Input has ticked but before CharacterMovement has ticked, so we can get the "intent" here. The control rotation is already
	// fresh new, but the delta is still old, so we calculate our own.
	const float controlYawDelta = pawn->GetCharacterMovementComponent()->GetControlRotation().y - pawn->GetCharacterMovementComponent()->GetLastFrameControlRotation().y;
	m_rootYaw += controlYawDelta;
	m_rootYaw = AFMath::NormalizeAngle(m_rootYaw);

	if (m_evaluationState == EAFAnimEvaluationState::Idle)
	{
		return;
	}

	switch (m_sourceState)
	{
	case EAFAnimSourceState::SingleAnim:
	{
		break;
	}
	case EAFAnimSourceState::Graph:
	{
		PreEvaluateGraph(deltaTime);
		break;
	}
	default:
	{
		break;
	}
	}
}

void AFAnimState::Tick(float deltaTime)
{
	std::shared_ptr<AFPlayerPawn> pawn = AFGame::GetGame()->GetScene().GetPlayerPawn();
	if (!pawn)
	{
		return;
	}

	// Increment distance traveled for distance matching.
	m_startRunDistanceTraveled += glm::length(pawn->GetCharacterMovementComponent()->GetLastLocationOffset());

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

void AFAnimState::CallFunctionByString(const std::string& functionName)
{
	switch (AFUtility::StringSwitch(functionName.c_str()))
	{
		case AFUtility::StringSwitch("OnStartRunEnter"):
		{
			OnStartRunEnter();
			break;
		}
		default:
		{

		}
	}
}

std::string AFAnimState::GetStartRunAnim() const
{
	return m_startRunAnim;
}

std::string AFAnimState::GetStartRunCurve_RootDistance() const
{
	return m_startRunCurve_rootDistance;
}

std::string AFAnimState::GetStartRunCurve_RootYaw() const
{
	return m_startRunCurve_rootYaw;
}

float AFAnimState::GetStartRunDistanceTraveled() const
{
	return m_startRunDistanceTraveled;
}

float AFAnimState::GetRootYaw() const
{
	return m_rootYaw;
}

void AFAnimState::SetRootYaw(float yaw)
{
	m_rootYaw = yaw;
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

void AFAnimState::PreEvaluateGraph(float deltaTime)
{
	if (!m_graph || !m_ownerMesh)
	{
		return;
	}

	// Mark all nodes dirty. This will allow for new evaluation of each of them.
	AFEvaluator::Get().ClearPreEvaluationState();

	AFEvaluator::Get().SetEvaluationMode(EAFEvaluationMode::PreEvaluate);
	m_graph->PreEvaluate(deltaTime);
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

	AFEvaluator::Get().SetEvaluationMode(EAFEvaluationMode::Evaluate);
	m_graph->Evaluate(deltaTime);

	const std::vector<std::shared_ptr<AFJoint>>& calculatedJoints = m_graph->GetFinalPose().GetJoints();
	const std::vector<std::shared_ptr<AFJoint>>& currentJoints = m_ownerMesh->GetMesh()->GetJoints();

	// Copy curves without resetting.
	std::unordered_map<std::string, float> curves = m_graph->GetFinalPose().GetCurvesValues();
	for (const auto& [name, value] : curves)
	{
		m_curves[name] = value;
	}

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

void AFAnimState::OnStartRunEnter()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	// How big the angle is towards target movement input from the root?
	const float angle = AFUtility::GetRootAngleTowardsMovementInput();

	std::vector<float> angles =
	{
		-180.0f,
		-90.0f,
		0.0f,
		90.0f,
		180.0f
	};

	// Which index per 90s angle it is?
	size_t index = AFMath::NearestIndex<float>(angles, angle);

	std::vector<std::string> startRunAnims =
	{
		"M_Neutral_Run_Reface_Start_F_L_180",
		"M_Neutral_Run_Reface_Start_F_L_090",
		"M_Neutral_Run_Start_F_Rfoot",
		"M_Neutral_Run_Reface_Start_F_R_090",
		"M_Neutral_Run_Reface_Start_F_R_180"
	};

	std::vector<std::string> startRunRootDistances =
	{
		"M_Neutral_Run_Reface_Start_F_L_180_rootDistance",
		"M_Neutral_Run_Reface_Start_F_L_090_rootDistance",
		"M_Neutral_Run_Start_F_Rfoot_rootDistance",
		"M_Neutral_Run_Reface_Start_F_R_090_rootDistance",
		"M_Neutral_Run_Reface_Start_F_R_180_rootDistance"
	};

	std::vector<std::string> startRunRootYaws =
	{
		"M_Neutral_Run_Reface_Start_F_L_180_rootYaw",
		"M_Neutral_Run_Reface_Start_F_L_090_rootYaw",
		"",
		"M_Neutral_Run_Reface_Start_F_R_090_rootYaw",
		"M_Neutral_Run_Reface_Start_F_R_180_rootYaw"
	};

	// Cache the start run anim name.
	m_startRunAnim = startRunAnims[index];

	// Cache the start run root distance curve name.
	m_startRunCurve_rootDistance = startRunRootDistances[index];

	// Cache the start run root yaw curve name.
	m_startRunCurve_rootYaw = startRunRootYaws[index];

	// Reset the distance traveled for distance-matching in startRun.
	m_startRunDistanceTraveled = 0.0f;
}
