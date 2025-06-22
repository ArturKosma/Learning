#pragma once
#include "AFGraphNode.h"

class AFGraphNode_MakeFloat : public AFGraphNodeCRTP<AFGraphNode_MakeFloat>
{
	AFCLASS(AFGraphNode_MakeFloat, "Make Float", "Compact|Color=Green");

public:

	AFPARAM(float, Input, "", "Input", "HidePin");
	AFPARAM(float, Output, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
