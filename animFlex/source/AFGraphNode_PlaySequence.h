#pragma once
#include "AFGraphNode.h"
#include <string>

#include "AFPose.h"

class AFGraphNode_PlaySequence : public AFGraphNode
{
public:

	AFGraphNode_PlaySequence();
	~AFGraphNode_PlaySequence() override;

	AFPARAM(std::string, animName, "", "Input", "HidePin|Dropdown_Anims");
	AFPARAM(AFPose, outputPose, "", "Output", "");

	void Evaluate(float deltaTime) override;

};
AFCLASS(AFGraphNode_PlaySequence, "Play Sequence", "")
