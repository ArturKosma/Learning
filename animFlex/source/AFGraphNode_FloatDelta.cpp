#include "AFGraphNode_FloatDelta.h"

void AFGraphNode_FloatDelta::Evaluate(float deltaTime)
{
	if (m_prev.has_value())
	{
		const float ret = m_floatInput - m_prev.value();
		m_floatOutput.SetValue(ret);
	}

	m_outputPose.SetValue(m_inputPose);
	m_prev= m_floatInput.GetValue();
}
