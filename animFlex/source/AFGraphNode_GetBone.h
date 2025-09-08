#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_GetBone : public AFGraphNodeCRTP<AFGraphNode_GetBone>
{
	AFCLASS(AFGraphNode_GetBone, "Get Bone", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(std::string, m_boneName, "", "Bone", "Input", "HidePin|Dropdown_Bones");
	AFPARAM(int, m_space, 0, "Space", "Input", "HidePin|EAFBoneSpace_Enum");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");
	AFPARAM(glm::vec3, m_outputLocation, (glm::vec3(0.0f, 0.0f, 0.0f)), "Location", "Output", "");
	AFPARAM(glm::vec3, m_outputRotation, (glm::vec3(0.0f, 0.0f, 0.0f)), "Rotation", "Output", "");

	void Evaluate(float deltaTime) override;
};
