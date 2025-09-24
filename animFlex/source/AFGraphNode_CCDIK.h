#pragma once
#include "AFGraphNode.h"

class AFGraphNode_CCDIK : public AFGraphNodeCRTP<AFGraphNode_CCDIK>
{
	AFCLASS(AFGraphNode_CCDIK, "CCDIK", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(std::string, m_rootBone, "", "Root Bone", "", "HidePin|Dropdown_Bones");
	AFPARAM(std::string, m_middleBone, "", "Middle Bone", "", "HidePin|Dropdown_Bones");
	AFPARAM(std::string, m_effectorBone, "", "Effector Bone", "", "HidePin|Dropdown_Bones");
	AFPARAM(glm::vec3, m_targetLoc, (glm::vec3(0.0f, 0.0f, 0.0f)), "Target Location", "Input", "");
	AFPARAM(glm::vec3, m_targetRot, (glm::vec3(0.0f, 0.0f, 0.0f)), "Target Rotation", "Input", "");
	AFPARAM(bool, m_active, true, "Active", "Input", "");
	AFPARAM(bool, m_applyEffectorRotation, true, "Apply Effector Rotation", "", "");
	AFPARAM(bool, m_applyTargetRotation, false, "Apply Target Rotation", "", "");
	AFPARAM(bool, m_useActivationPosition, false, "Use Activation Position", "", "");
	AFPARAM(float, m_blendInTime, 0.0f, "Blend In Time", "", "");
	AFPARAM(float, m_blendOutTime, 0.25f, "Blend Out Time", "", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;
	void OnBecomeRelevant() override;

private:

	std::shared_ptr<class AFSkeletalMeshComponent> m_mesh = nullptr;

	float m_currentAlpha = 0.0f;
	AFPose m_blendedPose = {};

	glm::vec3 m_activationPosition = glm::vec3(0.0f);
	glm::vec3 m_activationRotation = glm::vec3(0.0f);
	bool m_lastFrameActive = false;
};
