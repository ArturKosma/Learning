#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_BlendByBool : public AFGraphNodeCRTP<AFGraphNode_BlendByBool>
{
	AFCLASS(AFGraphNode_BlendByBool, "Blend by Bool", "");

public:

	AFPARAM(AFPose, blendBool_inputPoseA, "", "A", "Input", "");
	AFPARAM(AFPose, blendBool_inputPoseB, "", "B", "Input", "");
	AFPARAM(bool, blendBool_useA, true, "Use A", "Input", "");
	AFPARAM(AFPose, blendBool_outputPose, "", "", "Output", "");
	AFPARAM(float, blendBool_blendTime, 0.25f, "Blend Time", "", "");

	void Evaluate(float deltaTime) override;
};
