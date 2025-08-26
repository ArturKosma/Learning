#pragma once
#include "AFGraphNode.h"

class AFGraphNode_FloatDelta : public AFGraphNodeCRTP<AFGraphNode_FloatDelta>
{
	AFCLASS(AFGraphNode_FloatDelta, "Float Delta", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(float, m_floatInput, 0.0f, "", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");
	AFPARAM(float, m_floatOutput, 0.0f, "", "Output", "");

	void Evaluate(float deltaTime) override;

private:

	std::optional<float> m_prev;
};
