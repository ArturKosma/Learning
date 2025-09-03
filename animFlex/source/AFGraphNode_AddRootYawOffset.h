#pragma once
#include "AFGraphNode.h"

class AFGraphNode_AddRootYawOffset : public AFGraphNodeCRTP<AFGraphNode_AddRootYawOffset>
{
	AFCLASS(AFGraphNode_AddRootYawOffset, "Add Root Yaw Offset", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "Input", "Input", "");
	AFPARAM(float, m_offset, 0.0f, "Offset", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};
