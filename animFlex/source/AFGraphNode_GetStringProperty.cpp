#include "AFGraphNode_GetStringProperty.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFGraphNode_State.h"
#include "AFPlayerPawn.h"
#include "AFStateClass_Pivot.h"

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

void AFGraphNode_GetStringProperty::OnBecomeRelevant()
{
	m_open = true;
}

void AFGraphNode_GetStringProperty::EvalImpl(float deltaTime)
{
	if (m_charMovement.expired() || m_animState.expired())
	{
		return;
	}

	const std::string& context = GetNodeContext();
	std::shared_ptr<AFGraphNode> contextNode = AFGraphNodeRegistry::Get().GetNode(context);

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
		case EAFStringProperties::StopRunAnim:
		{
			ret = m_animState.lock()->GetStopRunAnim();
			break;
		}
		case EAFStringProperties::StopRunCurve_RootSpeed:
		{
			ret = m_animState.lock()->GetStopRunCurve_RootSpeed();
			break;
		}
		case EAFStringProperties::RotateInPlaceAnim:
		{
			ret = m_animState.lock()->GetRotateInPlaceAnim();
			break;
		}
		case EAFStringProperties::PivotAnim:
		{
			AFGraphNode_State* pivotState = std::dynamic_pointer_cast<AFGraphNode_State>(contextNode).get();
			if (!pivotState)
			{
				break;
			}

			AFStateClass_Pivot* pivotStateObj = std::dynamic_pointer_cast<AFStateClass_Pivot>(pivotState->GetStateObj()).get();
			if (!pivotStateObj)
			{
				break;
			}

			ret = pivotStateObj->GetPivotAnim();
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
