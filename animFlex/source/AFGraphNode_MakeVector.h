#pragma once
#include "AFGraphNode.h"

class AFGraphNode_MakeVector : public AFGraphNodeCRTP<AFGraphNode_MakeVector>
{
	AFCLASS(AFGraphNode_MakeVector, "Make Vector", "Compact")

public:

	AFPARAM(float, m_x, 0.0f, "X", "Input", "");
	AFPARAM(float, m_y, 0.0f, "Y", "Input", "");
	AFPARAM(float, m_z, 0.0f, "Z", "Input", "");
	AFPARAM(glm::vec3, m_outputVector, (glm::vec3(0.0f, 0.0f, 0.0f)), "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};
