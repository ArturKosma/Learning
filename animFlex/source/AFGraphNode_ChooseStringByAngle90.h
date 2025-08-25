#pragma once

#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_ChooseStringByAngle90 : public AFGraphNodeCRTP<AFGraphNode_ChooseStringByAngle90>
{
	AFCLASS(AFGraphNode_ChooseStringByAngle90, "Choose String By Angle 90", "")

public:

	AFPARAM(std::string, chooseByAngle_m180, {}, "-180", "Input", "");
	AFPARAM(std::string, chooseByAngle_m90, {}, "-90", "Input", "");
	AFPARAM(std::string, chooseByAngle_0, {}, "0", "Input", "");
	AFPARAM(std::string, chooseByAngle_90, {}, "90", "Input", "");
	AFPARAM(std::string, chooseByAngle_180, {}, "180", "Input", "");
	AFPARAM(float, chooseByAngle_angle, 0.0f, "Angle", "Input", "");
	AFPARAM(std::string, chooseByAngle_outputString, {}, "", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	size_t m_chosenIndex = 0;
	const std::vector<float> m_anglesArray = { -180.0f, -90.0f, 0.0f, 90.0f, 180.0f };
	std::vector<const FAFParam<std::string>*> m_stringsArray = {};
};
