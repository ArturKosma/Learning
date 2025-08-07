#pragma once
#include "AFGraphNode.h"

class AFGraphNode_MakeFloat : public AFGraphNodeCRTP<AFGraphNode_MakeFloat>
{
	AFCLASS(AFGraphNode_MakeFloat, "Make Float", "Compact")

public:

	AFPARAM(float, Input, 0.0f, "", "Input", "HidePin");
	AFPARAM(float, Output, 0.0f, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
