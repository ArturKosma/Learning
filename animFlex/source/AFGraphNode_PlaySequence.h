#pragma once
#include "AFGraphNode.h"
#include <string>

#include "AFPose.h"

class AFGraphNode_PlaySequence : public AFGraphNodeCRTP<AFGraphNode_PlaySequence>
{
	AFCLASS(AFGraphNode_PlaySequence, "Play Sequence", "");

public:

	AFPARAM(std::string, animName, "", "Input", "HidePin|Dropdown_Anims");
	AFPARAM(AFPose, outputPose, "", "Output", "");

	void Evaluate(float deltaTime) override;

};
