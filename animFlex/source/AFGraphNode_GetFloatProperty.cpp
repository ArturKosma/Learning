#include "AFGraphNode_GetFloatProperty.h"

#include "AFGame.h"
#include "AFGraphNode_StateCond.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

void AFGraphNode_GetFloatProperty::Init()
{
	m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
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

void AFGraphNode_GetFloatProperty::OnReset()
{
	m_distanceTraveled = 0.0f;
	m_previousPlayerLocation = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetLocation();

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
	case EAFFloatProperties::DistanceTraveled:
	{
		const glm::vec3 currentLoc = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetLocation();
		m_distanceTraveled = m_distanceTraveled + glm::length(currentLoc - m_previousPlayerLocation);
		m_previousPlayerLocation = currentLoc;

		ret = m_distanceTraveled;
		break;
	}
	case EAFFloatProperties::AngleTowardsMovementInput:
	{
		const glm::vec3& movementInput = glm::normalize(m_charMovement.lock()->GetLastPositiveMovementInput());
		const glm::quat& actorWorldRotation = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetRotationQuat();
		const glm::vec3& actorWorldForward = glm::normalize(actorWorldRotation * glm::vec3(0.0, 0.0f, 1.0f));

		const float angle = AFMath::SignedAngleBetweenVectors(movementInput, actorWorldForward, glm::vec3(0.0f, 1.0f, 0.0f));
		ret = angle;
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
