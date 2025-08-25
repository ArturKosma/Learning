#pragma once
#include "AFGraphNode.h"

class AFGraphNode_MakeString : public AFGraphNodeCRTP<AFGraphNode_MakeString>
{
	AFCLASS(AFGraphNode_MakeString, "Make String", "Compact")

public:

	AFPARAM(std::string, Input, "", "", "Input", "HidePin");
	AFPARAM(std::string, Output, "", "", "Output", "");

	void Evaluate(float deltaTime) override;
};
