#pragma once
#include "AFGraphNode.h"

class AFGraphNode_Abs : public AFGraphNodeCRTP<AFGraphNode_Abs>
{
	AFCLASS(AFGraphNode_Abs, "Absolute", "")

public:

	AFPARAM(float, m_input, 0.0f, "Value", "Input", "");
	AFPARAM(float, m_output, 0.0f, "Abs", "Output", "");

	void Evaluate(float deltaTime) override;

};
