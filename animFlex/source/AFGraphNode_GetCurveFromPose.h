#pragma once
#include "AFGraphNode.h"

class AFGraphNode_GetCurveFromPose : public AFGraphNodeCRTP<AFGraphNode_GetCurveFromPose>
{
	AFCLASS(AFGraphNode_GetCurveFromPose, "Get Curve From Pose", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(std::string, m_curveName, "", "", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");
	AFPARAM(float, m_curveValue, 0.0f, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
