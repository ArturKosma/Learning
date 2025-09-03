#pragma once
#include "AFGraphNode.h"

class AFGraphNode_Print : public AFGraphNodeCRTP<AFGraphNode_Print>
{
	AFCLASS(AFGraphNode_Print, "Print", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "Pose", "Input", "");
	AFPARAM(bool, m_printFloat, false, "Print Float", "", "");
	AFPARAM(bool, m_printString, false, "Print String", "", "");
	AFPARAM(float, m_inputFloat, 0.0f, "Input Float", "Input", "HidePin|HideControl|Show=m_printFloat");
	AFPARAM(std::string, m_inputString, "Input String", "", "Input", "HidePin|HideControl|Show=m_printString");
	AFPARAM(AFPose, m_outputPose, {}, "Output", "Output", "");

	void Evaluate(float deltaTime) override;
};
