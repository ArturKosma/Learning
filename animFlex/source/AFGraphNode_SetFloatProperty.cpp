#include "AFGraphNode_SetFloatProperty.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFPlayerPawn.h"

void AFGraphNode_SetFloatProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_SetFloatProperty::Evaluate(float deltaTime)
{
	if (m_charMovement.expired())
	{
		return;
	}

	switch (static_cast<EAFSetFloatProperties>(m_propertyEnum.GetValue()))
	{
	case EAFSetFloatProperties::RootYaw:
	{
		m_animState.lock()->SetRootYaw(m_floatProperty.GetValue());
		break;
	}
	default:
	{
		
		break;
	}
	}

	m_outputPose.SetValue(m_inputPose.GetValue());
}
