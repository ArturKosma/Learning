#pragma once
#include "AFGraphNode.h"

class AFGraphNode_Clamp : public AFGraphNodeCRTP<AFGraphNode_Clamp>
{
	AFCLASS(AFGraphNode_Clamp, "Clamp", "")

public:

	AFPARAM(float, m_value, 0.0f, "Input", "Input", "");
	AFPARAM(float, m_min, 0.0f, "Min", "Input", "");
	AFPARAM(float, m_max, 0.0f, "Max", "Input", "");
	AFPARAM(float, m_result, 0.0f, "Result", "Output", "");

	void Evaluate(float deltaTime) override;
};