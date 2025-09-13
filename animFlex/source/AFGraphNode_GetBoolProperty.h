#pragma once
#include "AFGraphNode.h"

AFENUM()
enum class EAFBoolProperties
{
	LeftFootLocked,
	RightFootLocked
};

class AFGraphNode_GetBoolProperty : public AFGraphNodeCRTP<AFGraphNode_GetBoolProperty>
{
	AFCLASS(AFGraphNode_GetBoolProperty, "Get Bool Property", "")

public:

	AFPARAM(int, m_propertyEnum, 0, "Property Name", "Input", "HidePin|EAFBoolProperties_Enum");
	AFPARAM(bool, m_boolProperty, false, "Output", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	std::weak_ptr<class AFCharacterMovementComponent> m_charMovement = {};
	std::weak_ptr<class AFAnimState> m_animState = {};
};
