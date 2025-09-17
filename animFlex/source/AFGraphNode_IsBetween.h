#pragma once
#include "AFGraphNode.h"

class AFGraphNode_IsBetween : public AFGraphNodeCRTP<AFGraphNode_IsBetween>
{
	AFCLASS(AFGraphNode_IsBetween, "Is Between", "")

public:

	AFPARAM(float, m_a, 0.0f, "Value", "Input", "");
	AFPARAM(float, m_min, 0.0f, "Min", "Input", "");
	AFPARAM(float, m_max, 0.0f, "Max", "Input", "");
	AFPARAM(bool, m_inclusive, false, "", "", "");
	AFPARAM(bool, m_out, false, "Result", "Output", "");

	void Evaluate(float deltaTime) override;
};
