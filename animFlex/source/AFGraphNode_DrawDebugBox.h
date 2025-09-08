#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_DrawDebugBox : public AFGraphNodeCRTP<AFGraphNode_DrawDebugBox>
{
	AFCLASS(AFGraphNode_DrawDebugBox, "Draw Debug Box", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(glm::vec3, m_location, (glm::vec3(0.0f, 0.0f, 0.0f)), "Location", "Input", "");
	AFPARAM(glm::vec3, m_rotation, (glm::vec3(0.0f, 0.0f, 0.0f)), "Rotation", "Input", "");
	AFPARAM(float, m_size, 10.0f, "Size", "", "");
	AFPARAM(float, m_lifetime, 0.0f, "Lifetime", "", "");
	AFPARAM(int, m_color, 0, "Color", "Input", "HidePin|EAFColor_Enum");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Evaluate(float deltaTime) override;
};
