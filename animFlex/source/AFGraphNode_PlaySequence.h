#pragma once
#include "AFGraphNode.h"
#include <string>

#include "AFPose.h"

class AFGraphNode_PlaySequence : public AFGraphNode
{
	AFCLASS(AFGraphNode_PlaySequence, "Play Sequence")

public:

	AFPARAM(std::string, animName, "", "Input");
	AFPARAM(AFPose, outputPose, "", "Output");

	void Evaluate(float deltaTime) override;

};
