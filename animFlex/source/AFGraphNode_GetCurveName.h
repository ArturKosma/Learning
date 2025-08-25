#pragma once
#include "AFGraphNode.h"

class AFGraphNode_GetCurveName : public AFGraphNodeCRTP<AFGraphNode_GetCurveName>
{
	AFCLASS(AFGraphNode_GetCurveName, "Get Curve Name", "Compact")

public:

	AFPARAM(std::string, Input, "", "", "Input", "HidePin|Dropdown_Curves");
	AFPARAM(std::string, Output, "", "", "Output", "");

	void OnUpdate() override;
	void Evaluate(float deltaTime) override;
};
