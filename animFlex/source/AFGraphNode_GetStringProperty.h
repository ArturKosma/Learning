#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFStringProperties
{
	StartRunAnim,
	StartRunCurve_RootDistance,
	StartRunCurve_RootYaw,
	StopRunAnim,
	StopRunCurve_RootSpeed,
	RotateInPlaceAnim,
	PivotAnim,
};

class AFGraphNode_GetStringProperty : public AFGraphNodeCRTP<AFGraphNode_GetStringProperty>
{
	AFCLASS(AFGraphNode_GetStringProperty, "Get String Property", "")

public:

	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFStringProperties_Enum");
	AFPARAM(std::string, m_stringProperty, "", "Output", "Output", "");
	AFPARAM(bool, m_once, false, "Evaluate Once When Relevant", "", "");

	void Init() override;
	void Evaluate(float deltaTime) override;
	void OnBecomeRelevant() override;

private:

	bool m_open = true;

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};
	std::weak_ptr<class AFAnimState> m_animState = {};

	void EvalImpl(float deltaTime);
};