#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFFloatProperties
{
	VelocityLength,
	UserInputLength,
	RelevantAnimTimeRemain,
	DistanceTraveled,
	AngleTowardsMovementInput
};

class AFGraphNode_GetFloatProperty : public AFGraphNodeCRTP<AFGraphNode_GetFloatProperty>
{
	AFCLASS(AFGraphNode_GetFloatProperty, "Get Float Property", "")

public:

	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFFloatProperties_Enum");
	AFPARAM(float, m_floatProperty, 0.0f, "Output", "Output", "");
	AFPARAM(bool, m_once, false, "Evaluate Once When Relevant", "", "");

	void Init() override;
	void Evaluate(float deltaTime) override;
	void OnReset() override;

private:

	float m_distanceTraveled = 0.0f;
	bool m_open = true;

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};

	void EvalImpl(float deltaTime);
};