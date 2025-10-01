#include "AFAnimState.h"
#include "AFSkeletalMeshComponent.h"
#include <chrono>

#include "AFCharacterMovementComponent.h"
#include "AFContent.h"
#include "AFDeltaObject.h"
#include "AFEvaluator.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"
#include "AFTimerManager.h"
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

const AFPose& AFAnimState::GetPose() const
{
	return m_pose;
}

void AFAnimState::OnEventFromClip(const std::string& event)
{
	/*switch (AFUtility::StringSwitch(event.c_str()))
	{
		case AFUtility::StringSwitch("footPlant_l"):
		{
			glm::vec3 location = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);

			AFUtility::GetBone(m_pose,
				"foot_l",
				EAFBoneSpace::World,
				location, rotation);

			AFUtility::DrawDebugBox(location, 20.0f, 3.0f, EAFColor::Red);
			break;
		}
		case AFUtility::StringSwitch("footPlant_r"):
		{
			glm::vec3 location = glm::vec3(0.0f);
			glm::vec3 rotation = glm::vec3(0.0f);

			AFUtility::GetBone(m_pose,
				"foot_r",
				EAFBoneSpace::World,
				location, rotation);

			AFUtility::DrawDebugBox(location, 20.0f, 3.0f, EAFColor::Green);
			break;
		}
		default:
		{
			break;	
		}
	}*/
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
		case AFUtility::StringSwitch("OnStartRunTick"):
		{
			OnStartRunTick();
			break;
		}
		case AFUtility::StringSwitch("OnStopRunEnter"):
		{
			OnStopRunEnter();
			break;
		}
		case AFUtility::StringSwitch("OnStopRunTick"):
		{
			OnStopRunTick();
			break;
		}
		case AFUtility::StringSwitch("OnRotateInPlaceEnter"):
		{
			OnRotateInPlaceEnter();
			break;
		}
		case AFUtility::StringSwitch("OnRotateInPlaceTick"):
		{
			OnRotateInPlaceTick();
			break;
		}
		default:
		{

		}
	}
}

float AFAnimState::GetStartRunDistanceMatchingTime() const
{
	return m_startRunDistanceMatchingTime;
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

float AFAnimState::GetStartRunDifferenceToInput() const
{
	return m_startRunDifferenceToInput;
}

float AFAnimState::GetStartRunTimeSpent() const
{
	return m_startRunTimeSpent;
}

float AFAnimState::GetRootYaw() const
{
	return m_rootYaw;
}

void AFAnimState::SetRootYaw(float yaw)
{
	m_rootYaw = yaw;
}

bool AFAnimState::GetLeftFeetLocked() const
{
	return m_leftFeetLocked;
}

bool AFAnimState::GetRightFeetLocked() const
{
	return m_rightFeetLocked;
}

float AFAnimState::GetStopRunDistanceMatchingTime() const
{
	return m_stopRun_distanceMatchingTime;
}

std::string AFAnimState::GetStopRunAnim() const
{
	return m_stopRunAnim;
}

std::string AFAnimState::GetStopRunCurve_RootSpeed() const
{
	return m_stopRunCurve_rootSpeed;
}

float AFAnimState::GetStopRunDistanceRemaining() const
{
	return m_stopRun_distanceRemaining;
}

float AFAnimState::GetRotateInPlacePlayTime() const
{
	return m_rotateInPlace_playTime;
}

std::string AFAnimState::GetRotateInPlaceAnim() const
{
	return m_rotateInPlace_anim;
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

	for (size_t i = 0; i < currentJoints.size(); ++i)
	{
		currentJoints[i]->SetLocation(calculatedJoints[i]->GetLocation());
		currentJoints[i]->SetRotation(calculatedJoints[i]->GetRotation());
		currentJoints[i]->SetScale(calculatedJoints[i]->GetScale());

		currentJoints[i]->CalculateLocalTRSMatrix();
	}

	m_pose = m_graph->GetFinalPose();
	m_ownerMesh->GetMesh()->jointsDirty = true;
}

void AFAnimState::OnStartRunEnter()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	// Reset for how long are we in the start run state.
	m_startRunTimeSpent = 0.0f;

	// Reset the distance traveled for distance-matching in startRun.
	// We take last offset as the move has already happened at this point.
	m_startRunDistanceTraveled = glm::length(charMovement->GetLastLocationOffset());

	// Reset delta object with the yaw curve delta.
	AFDeltaObject::Get().ResetValue("startRun_rootYawDeltaCrv");

	// Reset difference to input.
	m_startRunDifferenceToInput = 0.0f;

	// Reset cached movement input.
	m_cachedLocalMovementInput = glm::vec3(0.0f);

	// Reset distance matching time value.
	m_startRunDistanceMatchingTime = 0.0f;
	m_startRunDistanceMatchingTimeOffset = 0.0f;
	m_startRunCurve_rootDistanceCrv = nullptr;
}

void AFAnimState::OnStartRunTick()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	// Increment distance traveled for distance matching.
	m_startRunDistanceTraveled += glm::length(charMovement->GetLastLocationOffset());

	const float deltaTime = AFTimerManager::GetDeltaTime();
	const glm::vec3 localMovementInput = glm::normalize(charMovement->GetLastLocalMovementInput());

	// For a bit of time in the beginning of start run, keep choosing the start run animation.
	// If it changes, we will blend to it.
	// This is to support WSAD, where you want diagonal movement and you don't press the buttons exactly simultaneously.
	// We use local movement input, to ignore camera rotating during start run. Rotating camera does not trigger re-direction.
	if (m_startRunTimeSpent < 0.15f && (localMovementInput != m_cachedLocalMovementInput))
	{
		m_cachedLocalMovementInput = localMovementInput;

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

		// Since we are overwriting the root distance curve,
		// and the curve is used to derive distance matching time,
		// we will get a snap. Thus, we need to find the time offset to match the time and prevent snapping.
		std::shared_ptr<AFFloatCurve> newRootDistanceCrv = AFContent::Get().FindAsset<AFFloatCurve>(m_startRunCurve_rootDistance.c_str());
		if (m_startRunCurve_rootDistanceCrv)
		{
			const float distanceMatchingTime = m_startRunCurve_rootDistanceCrv->SampleByValue(m_startRunDistanceTraveled);
			m_startRunDistanceMatchingTimeOffset = newRootDistanceCrv->SampleByValue(m_startRunDistanceTraveled) - distanceMatchingTime;
		}
		m_startRunCurve_rootDistanceCrv = newRootDistanceCrv;

		// Cache the start run root yaw curve name.
		m_startRunCurve_rootYaw = startRunRootYaws[index];
		m_startRunCurve_rootYawCrv = AFContent::Get().FindAsset<AFFloatCurve>(m_startRunCurve_rootYaw.c_str());

		// Reset the yaw delta crv, to not get crazy delta when curve changes.
		// Setting value recomputes delta, using old cached matching time acts like "looking behind",
		// so that next delta sampling with this curve will get us a realistic value.
		if (m_startRunCurve_rootYawCrv)
		{
			const float rootYaw = m_startRunCurve_rootYawCrv->SampleByTime(m_startRunDistanceMatchingTime);
			AFDeltaObject::Get().SetValue("startRun_rootYawDeltaCrv", rootYaw);
		}
	}

	if (!m_startRunCurve_rootDistanceCrv)
	{
		return;
	}

	// Cache the driver time for all animations and curves in start run.
	m_startRunDistanceMatchingTime = m_startRunCurve_rootDistanceCrv->SampleByValue(m_startRunDistanceTraveled);
	m_startRunDistanceMatchingTime -= m_startRunDistanceMatchingTimeOffset;

	// Modify root yaw by the anims curve.
	if (m_startRunCurve_rootYawCrv)
	{
		// Rate of change in the root yaw curve.
		// Integral of this gives us full rotation from the curve.
		const float rootYawDeltaCrv = -1.0f * AFDeltaObject::Get().SetValue("startRun_rootYawDeltaCrv",
			m_startRunCurve_rootYawCrv->SampleByTime(m_startRunDistanceMatchingTime));

		// Remaining angle between root and movement input direction.
		const float angleMovement = AFUtility::GetRootAngleTowardsMovementInput();

		// Remaining angle from the curve.
		const float curveMax = m_startRunCurve_rootYawCrv->SampleByTime(9999.0f);
		const float curveCurrent = m_startRunCurve_rootYawCrv->SampleByTime(m_startRunDistanceMatchingTime);
		const float angleCurve = -1.0f * (curveMax - curveCurrent + glm::epsilon<float>());

		// Rate of change modifier k, to fit the desired angle when it's in between authored anims.
		// It's either above or below 1.0f, to strengthen or weaken the rotation curve delta.
		const float ratio = angleMovement / angleCurve;
		const float k = glm::clamp(ratio, -5.0f, 5.0f);

		m_startRunDifferenceToInput = glm::abs(angleMovement - angleCurve);

		// Modified root yaw by the rotation from start run anim.
		m_rootYaw = AFMath::NormalizeAngle(m_rootYaw - (rootYawDeltaCrv * k));
	}

	m_startRunTimeSpent += deltaTime;
}

void AFAnimState::OnStopRunEnter()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	// Reset the distance traveled for distance-matching in stopRun.
	// We take last offset as the move has already happened at this point.
	m_stopRunDistanceTraveled = glm::length(charMovement->GetLastLocationOffset());

	// Which foot is up in the air?
	glm::vec3 rotDummy;
	glm::vec3 lFeetLoc = {};
	glm::vec3 rFeetLoc = {};
	AFUtility::GetBone(GetPose(), "foot_l", EAFBoneSpace::Global, lFeetLoc, rotDummy);
	AFUtility::GetBone(GetPose(), "foot_r", EAFBoneSpace::Global, rFeetLoc, rotDummy);
	const bool lFeetUp = lFeetLoc.y > rFeetLoc.y;

	// Choose stop run based on feet up.
	m_stopRunAnim = lFeetUp ? "M_Neutral_Run_Stop_F_Lfoot" : "M_Neutral_Run_Stop_F_Rfoot";

	// Choose stop run root distance curve based on feet up.
	m_stopRunCurve_rootDistance = m_stopRunAnim + "_rootDistance";
	m_stopRunCurve_rootDistanceCrv = AFContent::Get().FindAsset<AFFloatCurve>(m_stopRunCurve_rootDistance.c_str());
	if (!m_stopRunCurve_rootDistanceCrv)
	{
		return;
	}
	m_stopRunCurve_rootSpeed = m_stopRunAnim + "_rootSpeed";

	// Choose time of start for the stop run anim.
	// @todo Luckily both L and R stops begin decelerating at 0.66f,
	// @todo but I should usually pick time automatically.
	m_stopRun_beginTime = 0.66f;

	// Find how much distance will be covered by animation till complete stop.
	// This will be used in distance matching.
	// In the current setup stop runs have root distance inverted - ie.
	// the distance gets lower, the closer we are to 0 speed.
	m_stopRun_distToZero = m_stopRunCurve_rootDistanceCrv->SampleByTime(m_stopRun_beginTime);

	// Find how much distance will be covered by character movement till complete stop.
	const float speed = glm::length(charMovement->GetVelocity());
	const float decel = charMovement->GetDeceleration();
	m_stopRun_distToZeroCharMov = (speed * speed) / (2.0f * decel);
}

void AFAnimState::OnStopRunTick()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	if (!m_stopRunCurve_rootDistanceCrv)
	{
		return;
	}

	// Increment distance traveled for distance matching.
	m_stopRunDistanceTraveled += glm::length(charMovement->GetLastLocationOffset());

	// Map the distance traveled to the distance covered by animation.
	const float distanceTraveledMapped = AFMath::MapRangeClamped(m_stopRunDistanceTraveled,
		0.0f,
		m_stopRun_distToZeroCharMov,
		0.0f,
		m_stopRun_distToZero);

	// Find the distance matching time using distance traveled sampled from stop run's root distance curve.
	// In the current setup stop runs have root distance inverted - ie.
	// the distance gets lower, the closer we are to 0 speed.
	m_stopRun_distanceMatchingTime = m_stopRunCurve_rootDistanceCrv->SampleByValue(m_stopRun_distToZero - distanceTraveledMapped);
	m_stopRun_distanceRemaining = m_stopRun_distToZero - distanceTraveledMapped;
}

void AFAnimState::OnRotateInPlaceEnter()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	// How big the angle is towards last positive user input from the root?
	const float angle = AFUtility::GetRootAngleTowardsMovementInput();

	std::vector<float> angles =
	{
		-180.0f,
		-135.0f,
		-90.0f,
		-45.0f,
		45.0f,
		90.0f,
		135.0f,
		180.0f
	};

	// Which index per 45s angle it is?
	size_t index = AFMath::NearestIndex<float>(angles, angle);

	std::vector<std::string> rotateInPlaceAnims =
	{
		"M_Neutral_Stand_Turn_180_L",
		"M_Neutral_Stand_Turn_135_L",
		"M_Neutral_Stand_Turn_090_L",
		"M_Neutral_Stand_Turn_045_L",
		"M_Neutral_Stand_Turn_045_R",
		"M_Neutral_Stand_Turn_090_R",
		"M_Neutral_Stand_Turn_135_R",
		"M_Neutral_Stand_Turn_180_R"
	};

	std::vector<std::string> rotateInPlaceYaws =
	{
		"M_Neutral_Stand_Turn_180_L_rootYaw",
		"M_Neutral_Stand_Turn_135_L_rootYaw",
		"M_Neutral_Stand_Turn_090_L_rootYaw",
		"M_Neutral_Stand_Turn_045_L_rootYaw",
		"M_Neutral_Stand_Turn_045_R_rootYaw",
		"M_Neutral_Stand_Turn_090_R_rootYaw",
		"M_Neutral_Stand_Turn_135_R_rootYaw",
		"M_Neutral_Stand_Turn_180_R_rootYaw"
	};

	// Cache the rotate in place anim name.
	m_rotateInPlace_anim = rotateInPlaceAnims[index];

	// Cache the rotate in place root yaw curve name.
	m_rotateInPlace_rootYaw = rotateInPlaceYaws[index];

	// Find the root yaw curve.
	m_rotateInPlace_curve_rootYaw = AFContent::Get().FindAsset<AFFloatCurve>(m_rotateInPlace_rootYaw.c_str());

	// Reset the rotate in place play time.
	m_rotateInPlace_playTime = 0.0f;

	// Reset the delta of root yaw.
	AFDeltaObject::Get().ResetValue("rotateInPlace_rootYawDeltaCrv");
}

void AFAnimState::OnRotateInPlaceTick()
{
	const float deltaTime = AFTimerManager::GetDeltaTime();

	m_rotateInPlace_playTime += deltaTime;

	// Modify root yaw by the anims curve.
	if (m_rotateInPlace_curve_rootYaw)
	{
		// Rate of change in the root yaw curve.
		// Integral of this gives us full rotation from the curve.
		const float rootYawDeltaCrv = -1.0f * AFDeltaObject::Get().SetValue("rotateInPlace_rootYawDeltaCrv",
			m_rotateInPlace_curve_rootYaw->SampleByTime(m_rotateInPlace_playTime));

		// Remaining angle between root and movement input direction.
		const float angleMovement = AFUtility::GetRootAngleTowardsMovementInput();

		// Remaining angle from the curve.
		const float curveMax = m_rotateInPlace_curve_rootYaw->SampleByTime(9999.0f);
		const float curveCurrent = m_rotateInPlace_curve_rootYaw->SampleByTime(m_rotateInPlace_playTime);
		const float angleCurve = -1.0f * (curveMax - curveCurrent + glm::epsilon<float>());

		// Rate of change modifier k, to fit the desired angle when it's in between authored anims.
		// It's either above or below 1.0f, to strengthen or weaken the rotation curve delta.
		const float ratio = angleMovement / angleCurve;
		const float k = glm::clamp(ratio, -1.4f, 1.4f);

		// Modified root yaw by the rotation from rotate in place anim.
		m_rootYaw = AFMath::NormalizeAngle(m_rootYaw - (rootYawDeltaCrv * k));
	}
}
