#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFBoolOperators
{
	And,
	Or
};

class AFGraphNode_BoolOperator: public AFGraphNodeCRTP<AFGraphNode_BoolOperator>
{
	AFCLASS(AFGraphNode_BoolOperator, "Bool Operator", "")

public:

	AFPARAM(bool, m_a, false, "A", "Input", "");
	AFPARAM(bool, m_b, false, "B", "Input", "");
	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFBoolOperators_Enum");
	AFPARAM(bool, m_boolOperatorResult, false, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};