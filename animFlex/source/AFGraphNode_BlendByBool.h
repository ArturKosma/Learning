#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_BlendByBool : public AFGraphNodeCRTP<AFGraphNode_BlendByBool>
{
	AFCLASS(AFGraphNode_BlendByBool, "Blend by Bool", "");

public:

	AFPARAM(AFPose, blendBool_inputPoseA, "A", "Input", "");
	AFPARAM(AFPose, blendBool_inputPoseB, "B", "Input", "");
	AFPARAM(bool, blendBool_useA, "Use A", "Input", "");
	AFPARAM(AFPose, blendBool_outputPose, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
