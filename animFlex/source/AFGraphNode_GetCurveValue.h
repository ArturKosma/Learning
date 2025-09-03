#pragma once
#include "AFGraphNode.h"

class AFAnimState;

class AFGraphNode_GetCurveValue: public AFGraphNodeCRTP<AFGraphNode_GetCurveValue>
{
	AFCLASS(AFGraphNode_GetCurveValue, "Get Curve Value", "")

public:

	AFPARAM(std::string, m_curveName, "", "", "Input", "HidePin|Show=m_showPin");
	AFPARAM(float, m_curveValue, 0.0f, "Value", "Output", "");
	AFPARAM(bool, m_showPin, false, "Show Pin", "", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	std::shared_ptr<AFAnimState> m_animState = nullptr;
};
