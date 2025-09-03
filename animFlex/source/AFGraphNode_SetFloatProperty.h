#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFSetFloatProperties
{
	RootYaw
};

class AFGraphNode_SetFloatProperty : public AFGraphNodeCRTP<AFGraphNode_SetFloatProperty>
{
	AFCLASS(AFGraphNode_SetFloatProperty, "Set Float Property", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFSetFloatProperties_Enum");
	AFPARAM(float, m_floatProperty, 0.0f, "New Value", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};
	std::weak_ptr<class AFAnimState> m_animState = {};
};
