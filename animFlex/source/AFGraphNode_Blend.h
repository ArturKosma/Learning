#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_Blend : public AFGraphNode
{
	AFCLASS(AFGraphNode_Blend, "Blend")

public:

	AFPARAM(AFPose, inputPoseA, "A", "Input");
	AFPARAM(AFPose, inputPoseB, "B", "Input");
	AFPARAM(float, alpha, "Alpha", "Input");
	AFPARAM(AFPose, outputPose, "", "Output");

	void Evaluate(float deltaTime) override;
};
