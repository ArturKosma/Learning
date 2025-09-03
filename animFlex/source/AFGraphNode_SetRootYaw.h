#pragma once
#include "AFGraphNode.h"

class AFGraphNode_SetRootYaw : public AFGraphNodeCRTP<AFGraphNode_SetRootYaw>
{
	AFCLASS(AFGraphNode_SetRootYaw, "Set Root Yaw", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(float, m_yaw, 0.0f, "Yaw", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
