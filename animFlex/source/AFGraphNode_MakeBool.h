#pragma once
#include "AFGraphNode.h"

class AFGraphNode_MakeBool : public AFGraphNodeCRTP<AFGraphNode_MakeBool>
{
	AFCLASS(AFGraphNode_MakeBool, "Make Bool", "Compact")

public:

	AFPARAM(bool, Input, "", "Input", "HidePin");
	AFPARAM(bool, Output, "", "Output", "");

	void Evaluate(float deltaTime) override;
};