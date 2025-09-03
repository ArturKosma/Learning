#include "AFGraphNode_FloatDelta.h"

void AFGraphNode_FloatDelta::OnReset()
{
	m_prev = 0.0f;
}

void AFGraphNode_FloatDelta::Evaluate(float deltaTime)
{
	const float input = m_floatInput.GetValue();
	const float delta = input - m_prev;

	m_floatOutput.SetValue(delta);
	m_outputPose.SetValue(m_inputPose.GetValue());

	m_prev = input;
}
