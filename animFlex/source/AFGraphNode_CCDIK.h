#pragma once
#include "AFGraphNode.h"

class AFGraphNode_CCDIK : public AFGraphNodeCRTP<AFGraphNode_CCDIK>
{
	AFCLASS(AFGraphNode_CCDIK, "CCDIK", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(std::string, m_rootBone, "", "Root Bone", "Input", "");
	AFPARAM(std::string, m_middleBone, "", "Middle Bone", "Input", "");
	AFPARAM(std::string, m_effectorBone, "", "Effector Bone", "Input", "");
	AFPARAM(glm::vec3, m_targetLoc, (glm::vec3(1.0f, 5.5f, 666.777f)), "Target", "Input", "");
	AFPARAM(bool, m_active, true, "Active", "Input", "");
	AFPARAM(float, m_blendInTime, 0.0f, "Blend In Time", "", "");
	AFPARAM(float, m_blendOutTime, 0.25f, "Blend Out Time", "", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Evaluate(float deltaTime) override;

private:

	float m_currentAlpha = 1.0f;
};
