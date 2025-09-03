#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFFInterpType
{
	Value,
	Angle
};

class AFGraphNode_FInterpToConst: public AFGraphNodeCRTP<AFGraphNode_FInterpToConst>
{
	AFCLASS(AFGraphNode_FInterpToConst, "FInterp To Const", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(int, m_operation, 0, "Type", "Input", "HidePin|EAFFInterpType_Enum");
	AFPARAM(float, m_current, 0.0f, "Current", "Input", "");
	AFPARAM(float, m_target, 0.0f, "Target", "Input", "");
	AFPARAM(float, m_rate, 1.0f, "Rate", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");
	AFPARAM(float, m_step, 0.0f, "Step", "Output", "");

	void Evaluate(float deltaTime) override;
};