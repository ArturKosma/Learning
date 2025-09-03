#pragma once
#include "AFGraphNode.h"

class AFGraphNode_NormalizeAngle : public AFGraphNodeCRTP<AFGraphNode_NormalizeAngle>
{
	AFCLASS(AFGraphNode_NormalizeAngle, "Normalize Angle", "")

public:

	AFPARAM(float, m_value, 0.0f, "Input", "Input", "");
	AFPARAM(float, m_result, 0.0f, "Angle", "Output", "");

	void Evaluate(float deltaTime) override;
};