#include "AFStateClass_Pivot.h"
#include "AFAnimState.h"
#include "AFCharacterMovementComponent.h"
#include "AFContent.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"
#include "AFTimerManager.h"

bool AFStateClass_Pivot::CallFunctionByString(const std::string& funcName)
{
	switch (AFUtility::StringSwitch(funcName.c_str()))
	{
		case AFUtility::StringSwitch("OnPivotEnter"):
		{
			OnPivotEnter();
			return true;
		}
		case AFUtility::StringSwitch("OnPivotTick"):
		{
			OnPivotTick();
			return true;
		}
		default:
		{

		}
	}

	return false;
}

std::string AFStateClass_Pivot::GetPivotAnim() const
{
	return m_pivotAnim;
}

float AFStateClass_Pivot::GetPivotTime() const
{
	return m_pivotTime;
}

float AFStateClass_Pivot::GetNewAngleVsAnim() const
{
	return m_newAngleVsAnim;
}

void AFStateClass_Pivot::OnPivotEnter()
{
	// @note The condition to enter this state should probably be found at the character movement level,
	// because the condition checks current root vs desired direction, but at the condition stage
	// root is already rotated a little towards that direction, so I should probably register the intent earlier,
	// and take the last frame rotation into consideration.

	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	std::shared_ptr<AFAnimState> animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
	if (!animState)
	{
		return;
	}

	const FAFPivotData& pivotData = FindPivotData();

	m_pivotAnim = pivotData.anim;

	// Cache the pivot root yaw curve.
	m_pivotCurve_rootYaw = m_pivotAnim + "_rootYaw";
	m_pivotCurve_rootYawCrv = AFContent::Get().FindAsset<AFFloatCurve>(m_pivotCurve_rootYaw.c_str());

	// Cache pivot start time.
	m_pivotStartTime = pivotData.startTime;

	// Reset for how long are we in the pivot state.
	m_pivotTimeSpent = 0.0f;

	// Reset start time + time spent.
	m_pivotTime = 0.0f;

	// Reset delta yaw.
	m_previousYawFromCrv = 0.0f;
}

void AFStateClass_Pivot::OnPivotTick()
{
	std::shared_ptr<AFCharacterMovementComponent> charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	if (!charMovement)
	{
		return;
	}

	std::shared_ptr<AFAnimState> animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
	if (!animState)
	{
		return;
	}

	const float deltaTime = AFTimerManager::GetDeltaTime();
	m_pivotTimeSpent += deltaTime;

	// For a short period of time, keep updating the anim.
	// Usable when user presses two directional keys with a short interval, to pick an in between animation.
	if (m_pivotTimeSpent < 0.05f)
	{
		const FAFPivotData& pivotData = FindPivotData();
		if (pivotData.anim != m_pivotAnim)
		{
			m_pivotAnim = pivotData.anim;

			// Cache the pivot root yaw curve.
			m_pivotCurve_rootYaw = m_pivotAnim + "_rootYaw";
			m_pivotCurve_rootYawCrv = AFContent::Get().FindAsset<AFFloatCurve>(m_pivotCurve_rootYaw.c_str());

			// Cache pivot start time.
			m_pivotStartTime = pivotData.startTime;

			// Reset start time + time spent.
			m_pivotTime = 0.0f;

			// Reset delta yaw.
			m_previousYawFromCrv = 0.0f;
		}
	}

	m_pivotTime = m_pivotTimeSpent + m_pivotStartTime;

	// Modify root yaw by the anims curve.
	// Integral of this gives us full rotation from the curve.
	const float rootYawDeltaCrv = -1.0f * (m_pivotCurve_rootYawCrv->SampleByTime(m_pivotTime) - m_previousYawFromCrv);
	m_previousYawFromCrv = m_pivotCurve_rootYawCrv->SampleByTime(m_pivotTime);

	// Remaining angle between root and movement input direction.
	const float angleMovement = AFUtility::GetRootAngleTowardsMovementInput();

	// Remaining angle from the curve.
	const float curveMax = m_pivotCurve_rootYawCrv->SampleByTime(9999.0f);
	const float curveCurrent = m_pivotCurve_rootYawCrv->SampleByTime(m_pivotTime);
	const float angleCurve = -1.0f * (curveMax - curveCurrent + glm::epsilon<float>());

	// After initial juggling we need to know how much will we be off the correct heading after the animation ends.
	m_newAngleVsAnim = m_pivotTimeSpent < 0.05f ? 0.0f : AFMath::NormalizeAngle(angleMovement - angleCurve);

	// Rate of change modifier k, to fit the desired angle when it's in between authored anims.
	// It's either above or below 1.0f, to strengthen or weaken the rotation curve delta.
	const float ratio = angleMovement / angleCurve;
	const float k = glm::clamp(ratio, -2.0f, 2.0f);

	// Modified root yaw by the rotation from pivot anim.
	animState->SetRootYaw(AFMath::NormalizeAngle(animState->GetRootYaw() - (rootYawDeltaCrv * k)));
}

FAFPivotData AFStateClass_Pivot::FindPivotData() const
{
	// How big the angle is towards target movement input from the root?
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

	std::vector<std::string> pivotAnims =
	{
		"M_Neutral_Run_Turn_L_180_Rfoot",
		"M_Neutral_Run_Turn_L_135_Rfoot",
		"M_Neutral_Run_Turn_L_090_Rfoot",
		"M_Neutral_Run_Turn_L_045_Rfoot",
		"M_Neutral_Run_Turn_R_045_Lfoot",
		"M_Neutral_Run_Turn_R_090_Lfoot",
		"M_Neutral_Run_Turn_R_135_Lfoot",
		"M_Neutral_Run_Turn_R_180_Lfoot"
	};

	// Handpicked start times (we are not starting this animation at frame 0).
	// @todo is there a better way?
	// @todo Those anims are prepared for a motionmatching system originally,
	// @todo so maybe it's okay to just cut the not needed beginning?
	std::vector<float> pivotStartTimes =
	{
		0.70f,
		0.30f,
		0.77f,
		0.27f,
		0.27f,
		0.70f,
		0.30f,
		0.67f
	};

	// Cache the pivot anim name.
	return { pivotAnims[index], pivotStartTimes[index] };
}
