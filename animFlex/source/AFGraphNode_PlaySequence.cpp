#include "AFGraphNode_PlaySequence.h"
#include "AFContent.h"


void AFGraphNode_PlaySequence::OnUpdate()
{
	const std::string& animName = playseq_animName.GetValue();

	// No anim name.
	if (animName.empty())
	{
		return;
	}

	// Nothing changed, no fetch.
	if (m_animName == animName)
	{
		return;
	}

	// If there was a valid anim and the blend time is non-zero, we add on top of blendstack.
	const bool blendStack = !m_animName.empty() && playseq_blendTime > 0.0f;
	m_animName = animName;

	// Called upon anims fetch complete.
	auto onComplete = [this, blendStack](std::shared_ptr<AFAnimationClip> fetchedAnim)
		{
			if (blendStack)
			{
				// Blendstack evaluators.
				std::shared_ptr<FAFBlendStack_Evaluator> fromEvaluator = std::make_shared<FAFBlendStack_Evaluator>();
				fromEvaluator->clip = m_animClip;
				std::shared_ptr<FAFBlendStack_Evaluator> toEvaluator = std::make_shared<FAFBlendStack_Evaluator>();
				toEvaluator->clip = fetchedAnim;

				// Create new blend which will end up on the top of the stack.
				std::shared_ptr<FAFBlendStack_Blender> newBlender = std::make_shared<FAFBlendStack_Blender>();
				newBlender->duration = playseq_blendTime.GetValue();
				newBlender->t = 0.0f;

				// From is either simple evaluator of previous top of the blend stack, or a blender.
				std::shared_ptr<IAFBlendStack_Node> fromBlendOrEval = m_blendStack.empty() ?
					std::static_pointer_cast<IAFBlendStack_Node>(fromEvaluator) :
					std::static_pointer_cast<IAFBlendStack_Node>(m_blendStack.back());
				newBlender->from = fromBlendOrEval;
				newBlender->to = toEvaluator;
				newBlender->direction = EAFBlendDirection::Forward;
				m_blendStack.push_back(newBlender);
			}

			m_animClip = fetchedAnim;
		};

	// Fetch anim.
	AFContent::Get().FetchAsset<AFAnimationClip>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/",
		animName,
		onComplete, ".afanim");
}

void AFGraphNode_PlaySequence::Evaluate(float deltaTime)
{
	OnUpdate();

	if(!m_animClip)
	{
		return;
	}

	const float endTime = playseq_endTime <= 0.0f ? m_animClip->GetClipEndTime() : playseq_endTime;

	if (playseq_manualTime)
	{
		// Manual m_localTime.
		m_localTime = glm::clamp(playseq_manualTimeFloat.GetValue(), playseq_startTime.GetValue(), endTime - playseq_startTime);
	}
	else
	{
		// Increase time & sample anim.
		if (playseq_loop)
		{
			m_localTime = std::fmod(m_localTime + (deltaTime * playseq_playrate), endTime - playseq_startTime) + playseq_startTime;
		}
		else
		{
			m_localTime = glm::clamp(m_localTime + (deltaTime * playseq_playrate), playseq_startTime.GetValue(), endTime - playseq_startTime);
		}
	}

	// Register the clip sampling globally.
	// We need this information for things like RelevantAnimTimeRemaining.
	FAFStateSampling sampling;
	sampling.nodeId = m_nodeId;
	sampling.contextId = m_nodeContext;
	sampling.sampleTime = m_localTime;
	sampling.maxTime = endTime;
	AFEvaluator::Get().AddSamplingState(sampling);

	// Evaluate final pose from blend stack.
	if (!m_blendStack.empty())
	{
		FAFBlendStack_Blender* topBlender = m_blendStack.back().get();
		if (!topBlender)
		{
			return;
		}

		// Progress blend time.
		topBlender->ProgressBlendTime(deltaTime);

		// Evaluate top of the blend stack.
		// Each blender accesses previous blender, ultimately ending at simple evaluators.
		FAFBlendStackEvalParams params;
		params.time = m_localTime;
		params.rootLock = playseq_forceRootLock.GetValue();
		topBlender->Evaluate(deltaTime, const_cast<AFPose&>(playseq_outputPose.GetValue()), params);

		// If top blender is done blending, we drop whole stack.
		if (topBlender->HasFinished())
		{
			m_blendStack.clear();
		}
	}
	else
	{
		const_cast<AFPose&>(playseq_outputPose.GetValue()).ApplyClip(m_animClip, m_localTime, playseq_forceRootLock.GetValue());
	}
}

void AFGraphNode_PlaySequence::OnReset()
{
	m_localTime = 0.0f;
}
