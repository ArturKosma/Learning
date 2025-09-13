#include "AFGraphNode_GetBoolProperty.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetBoolProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_GetBoolProperty::Evaluate(float deltaTime)
{
	bool ret = false;

	switch (static_cast<EAFBoolProperties>(m_propertyEnum.GetValue()))
	{
		case EAFBoolProperties::LeftFootLocked:
		{
			ret = m_animState.lock()->GetLeftFeetLocked();
			break;
		}
		case EAFBoolProperties::RightFootLocked:
		{
			ret = m_animState.lock()->GetRightFeetLocked();
			break;
		}
		default:
		{
			break;
		}
	}

	return m_boolProperty.SetValue(ret);
}
