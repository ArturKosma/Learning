#pragma once

#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_ChooseByAngle90 : public AFGraphNodeCRTP<AFGraphNode_ChooseByAngle90>
{
	AFCLASS(AFGraphNode_ChooseByAngle90, "Choose By Angle 90", "")

public:

	AFPARAM(AFPose, chooseByAngle_m180, {}, "-180", "Input", "");
	AFPARAM(AFPose, chooseByAngle_m90, {}, "-90", "Input", "");
	AFPARAM(AFPose, chooseByAngle_0, {}, "0", "Input", "");
	AFPARAM(AFPose, chooseByAngle_90, {}, "90", "Input", "");
	AFPARAM(AFPose, chooseByAngle_180, {}, "180", "Input", "");
	AFPARAM(float, chooseByAngle_angle, 0.0f, "Angle", "Input", "");
	AFPARAM(AFPose, chooseByAngle_outputPose, {}, "", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	size_t m_chosenIndex = 0;
	const std::vector<float> m_anglesArray = { -180.0f, -90.0f, 0.0f, 90.0f, 180.0f };
	std::vector<const FAFParam<AFPose>*> m_posesArray = {};
};
