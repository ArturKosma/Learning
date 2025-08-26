#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFCurve
{
	rootDistance,
	rootYaw,
	rootYawDelta
};

class AFGraphNode_SetCurveValue : public AFGraphNodeCRTP<AFGraphNode_SetCurveValue>
{
	AFCLASS(AFGraphNode_SetCurveValue, "Set Curve Value", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(int, m_curveEnum, 0, "Curve", "Input", "HidePin|EAFCurve_Enum");
	AFPARAM(float, m_inputValue, 0.0f, "", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
