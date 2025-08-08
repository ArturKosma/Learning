#include "AFGraphNode_GetFloatProperty.h"

#include "AFGame.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetFloatProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
}

void AFGraphNode_GetFloatProperty::Evaluate(float deltaTime)
{
	if (m_charMovement.expired())
	{
		return;
	}

	float ret = 0.0f;

	switch (static_cast<EAFFloatProperties>(m_propertyEnum.GetValue()))
	{
		case EAFFloatProperties::VelocityLength:
		{
			ret = glm::length(m_charMovement.lock()->GetVelocity());
			break;
		}
		case EAFFloatProperties::DeltaAngle:
		{
			ret = 0.0f;
			break;
		}
		default:
		{
			ret = 0.0f;
			break;
		}
	}

	m_floatProperty.SetValue(ret);
}
