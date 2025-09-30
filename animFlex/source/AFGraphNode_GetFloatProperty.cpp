#include "AFGraphNode_GetFloatProperty.h"

#include "AFAnimState.h"
#include "AFGame.h"
#include "AFGraphNode_StateCond.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetFloatProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
}

void AFGraphNode_GetFloatProperty::Evaluate(float deltaTime)
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

void AFGraphNode_GetFloatProperty::OnBecomeRelevant()
{
	m_open = true;
}

void AFGraphNode_GetFloatProperty::EvalImpl(float deltaTime)
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
	case EAFFloatProperties::UserInputLength:
	{
		ret = glm::length(m_charMovement.lock()->GetMovementInput());
		break;
	}
	case EAFFloatProperties::RelevantAnimTimeRemain:
	{
		// Relevant anim time remaining requires the context to be a State Conditional node,
		// with a valid From transition.
		const std::string& nodeContext = m_nodeContext;
		if (nodeContext.empty())
		{
			break;
		}

		std::shared_ptr<AFGraphNode_StateCond> contextCond = std::dynamic_pointer_cast<AFGraphNode_StateCond>
			(AFGraphNodeRegistry::Get().GetNode(nodeContext));
		if (!contextCond)
		{
			break;
		}

		std::string from;
		std::string to;
		contextCond->GetConnectionPoints(from, to);

		// Valid from.
		if (from.empty())
		{
			break;
		}

		// Get cached evaluations that match the From context.
		std::vector<FAFStateSampling> samplings = AFEvaluator::Get().GetCachedSamplingState(from);

		// Return the one with least remaining time.
		// ie. blendspace with 8 anims, take the one which has the least remaining time.
		FAFStateSampling best = FAFStateSampling();
		float bestRemaining = std::numeric_limits<float>::max();
		for (const auto& sampling : samplings)
		{
			float remaining = sampling.maxTime - sampling.sampleTime;
			if (remaining < bestRemaining)
			{
				bestRemaining = remaining;
				best = sampling;
			}
		}

		ret = bestRemaining;

		break;
	}
	case EAFFloatProperties::StartRunDistanceTraveled:
	{
		ret = m_animState.lock()->GetStartRunDistanceTraveled();
		break;
	}
	case EAFFloatProperties::AngleTowardsMovementInput:
	{
		ret = AFUtility::GetAngleTowardsMovementInput();
		break;
	}
	case EAFFloatProperties::ControlYawDelta:
	{
		ret = m_charMovement.lock()->GetLastControlYawDelta();
		break;
	}
	case EAFFloatProperties::RootYaw:
	{
		ret = m_animState.lock()->GetRootYaw();
		break;
	}
	case EAFFloatProperties::RootAngleTowardsMovementInput:
	{
		ret = AFUtility::GetRootAngleTowardsMovementInput();
		break;
	}
	case EAFFloatProperties::RootAngleTowardsVelocity:
	{
		ret = AFUtility::GetRootAngleTowardsVelocity();
		break;
	}
	case EAFFloatProperties::AngleTowardsVelocity:
	{
		ret = AFUtility::GetAngleTowardsVelocity();
		break;
	}
	case EAFFloatProperties::StartRunDistanceMatchingTime:
	{
		ret = m_animState.lock()->GetStartRunDistanceMatchingTime();
		break;
	}
	case EAFFloatProperties::StartRunDifferenceToInput:
	{
		ret = m_animState.lock()->GetStartRunDifferenceToInput();
		break;
	}
	case EAFFloatProperties::StopRunDistanceMatchingTime:
	{
		ret = m_animState.lock()->GetStopRunDistanceMatchingTime();
		break;
	}
	case EAFFloatProperties::StopRunDistanceRemaining:
	{
		ret = m_animState.lock()->GetStopRunDistanceRemaining();
		break;
	}
	case EAFFloatProperties::StartRunTimeSpent:
	{
		ret = m_animState.lock()->GetStartRunTimeSpent();
		break;
	}
	case EAFFloatProperties::RotateInPlacePlayTime:
	{
		ret = m_animState.lock()->GetRotateInPlacePlayTime();
		break;
	}
	case EAFFloatProperties::PivotDistanceMatchingTime:
	{
		ret = m_animState.lock()->GetPivotDistanceMatchingTime();
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
