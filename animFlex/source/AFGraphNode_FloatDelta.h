#pragma once
#include "AFGraphNode.h"

class AFGraphNode_FloatDelta : public AFGraphNodeCRTP<AFGraphNode_FloatDelta>
{
	AFCLASS(AFGraphNode_FloatDelta, "Float Delta", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(float, m_floatInput, 0.0f, "Input", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");
	AFPARAM(float, m_floatOutput, 0.0f, "Delta", "Output", "");

	void OnBecomeRelevant() override;
	void Evaluate(float deltaTime) override;

private:

	float m_prev = 0.0f;
};
