#include "AFGraphNode_GetFloatProperty.h"

#include "AFGame.h"
#include "AFGraphNode_StateCond.h"
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
			if(nodeContext.empty())
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
		default:
		{
			ret = 0.0f;
			break;
		}
	}

	m_floatProperty.SetValue(ret);
}
