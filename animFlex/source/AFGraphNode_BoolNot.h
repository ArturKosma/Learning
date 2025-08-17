#pragma once
#include "AFGraphNode.h"

class AFGraphNode_BoolNot : public AFGraphNodeCRTP<AFGraphNode_BoolNot>
{
	AFCLASS(AFGraphNode_BoolNot, "Not", "")

public:

	AFPARAM(bool, m_boolNotInput, false, "", "Input", "");
	AFPARAM(bool, m_boolNotResult, false, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};