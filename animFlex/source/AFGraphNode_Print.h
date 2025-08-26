#pragma once
#include "AFGraphNode.h"

class AFGraphNode_Print : public AFGraphNodeCRTP<AFGraphNode_Print>
{
	AFCLASS(AFGraphNode_Print, "Print", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(bool, m_printFloat, false, "", "", "");
	AFPARAM(bool, m_printString, false, "", "", "");
	AFPARAM(float, m_inputFloat, 0.0f, "", "Input", "HidePin|HideControl|Show=m_printFloat");
	AFPARAM(std::string, m_inputString, "", "", "Input", "HidePin|HideControl|Show=m_printString");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
