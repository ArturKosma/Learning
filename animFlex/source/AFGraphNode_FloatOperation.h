#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFFloatOperations
{
	Add,
	Substract,
	Multiply,
	Divide,
	Modulo
};

class AFGraphNode_FloatOperation : public AFGraphNodeCRTP<AFGraphNode_FloatOperation>
{
	AFCLASS(AFGraphNode_FloatOperation, "Float Operation", "")

public:

	AFPARAM(float, m_a, 0.0f, "A", "Input", "");
	AFPARAM(float, m_b, 0.0f, "B", "Input", "");
	AFPARAM(int, m_operation, 0, "Operation", "Input", "HidePin|EAFFloatOperations_Enum");
	AFPARAM(float, m_result, 0.0f, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};