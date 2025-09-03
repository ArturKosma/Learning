#include "AFGraphNode_GetStringProperty.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetStringProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_GetStringProperty::Evaluate(float deltaTime)
{
	if (!m_open)
	{
		return;
	}

	EvalImpl(deltaTime);

	if (m_once)
	{
		m_open = false;
	}
}

void AFGraphNode_GetStringProperty::OnReset()
{
	m_open = true;
}

void AFGraphNode_GetStringProperty::EvalImpl(float deltaTime)
{
	if (m_charMovement.expired() || m_animState.expired())
	{
		return;
	}

	std::string ret = "";

	switch (static_cast<EAFStringProperties>(m_propertyEnum.GetValue()))
	{
		case EAFStringProperties::StartRunAnim:
		{
			ret = m_animState.lock()->GetStartRunAnim();
			break;
		}
		case EAFStringProperties::StartRunCurve_RootDistance:
		{
			ret = m_animState.lock()->GetStartRunCurve_RootDistance();
			break;
		}
		case EAFStringProperties::StartRunCurve_RootYaw:
		{
			ret = m_animState.lock()->GetStartRunCurve_RootYaw();
			break;
		}
		default:
		{
			ret = "";
			break;
		}
	}

	m_stringProperty.SetValue(ret);
}
