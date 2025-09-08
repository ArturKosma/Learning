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
			// @todo Create a separate function returning step and let choose on the node if its step.
			ret = AFMath::FInterpToConst(m_current.GetValue(), m_target.GetValue(), m_rate.GetValue(), AFTimerManager::GetDeltaTime());
			break;
		}
		case EAFFInterpType::Angle:
		{
			// @todo Create a separate function returning step and let choose on the node if its step.
			ret = AFMath::FInterpToAngle(m_current.GetValue(), m_target.GetValue(), m_rate.GetValue(), AFTimerManager::GetDeltaTime());
			break;
		}
	}

	m_step.SetValue(ret);
}
