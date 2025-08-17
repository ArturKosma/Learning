#include "AFGraphNode_PlaySequence.h"

#include "AFContent.h"

void AFGraphNode_PlaySequence::Init()
{

}

void AFGraphNode_PlaySequence::OnUpdate()
{
	auto onComplete = [this](std::shared_ptr<AFAnimationClip> fetchedAnim)
		{
			m_animClip = fetchedAnim;
			m_localTime = 0.0f;
		};

	AFContent::Get().FetchAsset<AFAnimationClip>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/", 
		playseq_animName.GetValue().c_str(),
		onComplete);
}

void AFGraphNode_PlaySequence::Evaluate(float deltaTime)
{
	if(!m_animClip)
	{
		return;
	}

	const float endTime = playseq_endTime <= 0.0f ? m_animClip->GetClipEndTime() : playseq_endTime;

	// Increase time & sample anim.
	if (playseq_loop)
	{
		m_localTime = std::fmod(m_localTime + (deltaTime * playseq_playrate), endTime - playseq_startTime) + playseq_startTime;
	}
	else
	{
		m_localTime = glm::clamp(m_localTime + (deltaTime * playseq_playrate), playseq_startTime.GetValue(), endTime - playseq_startTime);
	}

	const_cast<AFPose&>(playseq_outputPose.GetValue()).ApplyClip(m_animClip, m_localTime, playseq_forceRootLock);

	// Register the clip sampling globally.
	// We need this information for things like RelevantAnimTimeRemaining.
	FAFStateSampling sampling;
	sampling.nodeId = m_nodeId;
	sampling.contextId = m_nodeContext;
	sampling.sampleTime = m_localTime;
	sampling.maxTime = endTime;
	AFEvaluator::Get().AddSamplingState(sampling);
}

void AFGraphNode_PlaySequence::OnReset()
{
	m_localTime = 0.0f;
}
