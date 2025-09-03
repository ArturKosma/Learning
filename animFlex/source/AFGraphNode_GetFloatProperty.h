#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFFloatProperties
{
	VelocityLength,
	UserInputLength,
	RelevantAnimTimeRemain,
	StartRunDistanceTraveled,
	AngleTowardsMovementInput,
	ControlYawDelta,
	RootYaw,
	RootAngleTowardsMovementInput,
	RootAngleTowardsVelocity,
	AngleTowardsVelocity,
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

	bool m_open = true;

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};
	std::weak_ptr<class AFAnimState> m_animState = {};

	void EvalImpl(float deltaTime);
};
