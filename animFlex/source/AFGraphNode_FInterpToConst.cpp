#include "AFGraphNode_FInterpToConst.h"

#include "AFMath.h"

void AFGraphNode_FInterpToConst::Evaluate(float deltaTime)
{
	m_outputPose.SetValue(m_inputPose.GetValue());

	float ret = 0.0f;

	switch (static_cast<EAFFInterpType>(m_operation.GetValue()))
	{
		case EAFFInterpType::Value:
		{
			ret = AFMath::FInterpTo(m_current.GetValue(), m_target.GetValue(), m_rate.GetValue(), AFTimerManager::GetDeltaTime());
			break;
		}
		case EAFFInterpType::Angle:
		{
			ret = AFMath::FInterpToAngle(m_current.GetValue(), m_target.GetValue(), m_rate.GetValue(), AFTimerManager::GetDeltaTime());
			break;
		}
	}

	m_step.SetValue(ret);
}
