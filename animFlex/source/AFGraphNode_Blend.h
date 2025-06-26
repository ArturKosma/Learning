#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_Blend : public AFGraphNodeCRTP<AFGraphNode_Blend>
{
	AFCLASS(AFGraphNode_Blend, "Blend", "");

public:

	AFGraphNode_Blend();

	AFPARAM(AFPose, blend_inputPoseA, "A", "Input", "");
	AFPARAM(AFPose, blend_inputPoseB, "B", "Input", "");
	AFPARAM(float, blend_alpha, "Alpha", "Input", "");
	AFPARAM(AFPose, blend_outputPose, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
