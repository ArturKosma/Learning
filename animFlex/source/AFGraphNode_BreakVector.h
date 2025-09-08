#pragma once
#include "AFGraphNode.h"

class AFGraphNode_BreakVector : public AFGraphNodeCRTP<AFGraphNode_BreakVector>
{
	AFCLASS(AFGraphNode_BreakVector, "Break Vector", "Compact")

public:

	AFPARAM(glm::vec3, m_inputVector, (glm::vec3(0.0f, 0.0f, 0.0f)), "Input", "Input", "");
	AFPARAM(float, m_x, 0.0f, "X", "Output", "");
	AFPARAM(float, m_y, 0.0f, "Y", "Output", "");
	AFPARAM(float, m_z, 0.0f, "Z", "Output", "");

	void Evaluate(float deltaTime) override;
};
