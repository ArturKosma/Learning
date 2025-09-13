#include "AFGraphNode_InterpRootYawTowardsVelocity.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

void AFGraphNode_InterpRootYawTowardsVelocity::Init()
{
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_InterpRootYawTowardsVelocity::Evaluate(float deltaTime)
{
	const AFPose& pose = m_inputPose.GetValue();

	if (m_animState)
	{
		const float currentRootYaw = m_animState->GetRootYaw();
		const float authority = m_authority.GetValue() ? glm::abs(1.0f - pose.GetCurvesValues().at("rootYawAuthority")) : 1.0f;
		const float step = 
			AFMath::FInterpToAngle(AFMath::NormalizeAngle(currentRootYaw),
				-1.0f * AFUtility::GetAngleTowardsVelocity(),
				m_rate.GetValue(),
				deltaTime);

		// Add new root yaw step by step.
		const float newRootYaw = currentRootYaw + (authority * step);
		m_animState->SetRootYaw(newRootYaw);
	}

	m_outputPose.SetValue(pose);
}
