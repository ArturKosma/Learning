#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_BlendByBool : public AFGraphNode
{
public:
	AFGraphNode_BlendByBool();

	AFPARAM(AFPose, inputPoseA, "A", "Input", "");
	AFPARAM(AFPose, inputPoseB, "B", "Input", "");
	AFPARAM(bool, useA, "Use A", "Input", "");
	AFPARAM(AFPose, outpusePose, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
AFCLASS(AFGraphNode_BlendByBool, "Blend by Bool")