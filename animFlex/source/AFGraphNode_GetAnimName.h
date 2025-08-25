#pragma once
#include "AFGraphNode.h"

class AFGraphNode_GetAnimName : public AFGraphNodeCRTP<AFGraphNode_GetAnimName>
{
	AFCLASS(AFGraphNode_GetAnimName, "Get Anim Name", "Compact")

public:

	AFPARAM(std::string, Input, "", "", "Input", "HidePin|Dropdown_Anims");
	AFPARAM(std::string, Output, "", "", "Output", "");

	void OnUpdate() override;
	void Evaluate(float deltaTime) override;

private:

	std::shared_ptr<AFAnimationClip> m_animClip = nullptr;
};
