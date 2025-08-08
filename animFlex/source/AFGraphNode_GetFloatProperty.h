#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFFloatProperties
{
	VelocityLength,
	DeltaAngle
};

class AFGraphNode_GetFloatProperty : public AFGraphNodeCRTP<AFGraphNode_GetFloatProperty>
{
	AFCLASS(AFGraphNode_GetFloatProperty, "Get Float Property", "")

public:

	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFFloatProperties_Enum");
	AFPARAM(float, m_floatProperty, 0.0f, "Output", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};
};