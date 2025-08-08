#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFCompareTypes
{
	Greater,
	GreaterEqual,
	Equal,
	NonEqual,
	Lesser,
	LesserEqual
};

class AFGraphNode_Compare : public AFGraphNodeCRTP<AFGraphNode_Compare>
{
	AFCLASS(AFGraphNode_Compare, "Compare", "")

public:

	AFPARAM(float, m_a, 0.0f, "A", "Input", "");
	AFPARAM(float, m_b, 0.0f, "B", "Input", "");
	AFPARAM(int, m_compareType, 0, "Compare Type", "Input", "HidePin|EAFCompareTypes_Enum");
	AFPARAM(bool, m_compareResult, false, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};
