#include "AFGraphNode_FloatDelta.h"

void AFGraphNode_FloatDelta::OnBecomeRelevant()
{
	m_prev = 0.0f;
}

void AFGraphNode_FloatDelta::Evaluate(float deltaTime)
{
	m_outputPose.SetValue(m_inputPose.GetValue());

	const float input = m_floatInput.GetValue();
	const float delta = input - m_prev;

	m_floatOutput.SetValue(delta);

	m_prev = input;
}
