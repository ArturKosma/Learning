#include "AFGraphNode_PlaySequence.h"
#include <string_view>
#include "AFContent.h"

void AFGraphNode_PlaySequence::Init()
{

}

void AFGraphNode_PlaySequence::OnUpdate()
{
	if (playseq_animName.GetValue().empty())
	{
		return;
	}

	// Called upon curves fetch complete.
	auto onCompleteCurves = [this](std::vector<std::shared_ptr<AFFloatCurve>> fetchedCurves)
		{
			if (!m_animClip)
			{
				return;
			}

			const std::string prefix = m_animClip->GetClipName() + "_";
			const std::string suffix = ".json";

			for (std::shared_ptr<AFFloatCurve> crv : fetchedCurves)
			{
				std::string name = crv->GetName();
				std::string_view sv(name);
				sv.remove_prefix(prefix.size());
				sv.remove_suffix(suffix.size());

				// Add each curve.
				m_animClip->AddCurve(std::string(sv), crv);
			}
		};

	// Called upon anims fetch complete.
	auto onComplete = [this, onCompleteCurves](std::shared_ptr<AFAnimationClip> fetchedAnim)
		{
			m_animClip = fetchedAnim;
			m_localTime = 0.0f;

			// Fetch curves.
			AFContent::Get().FetchAssets<AFFloatCurve>("https://api.github.com/repos/ArturKosma/assets/contents/curves?ref=main",
				"https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/curves/",
				playseq_animName.GetValue(),
				onCompleteCurves);
		};

	// Fetch anim.
	AFContent::Get().FetchAsset<AFAnimationClip>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/", 
		playseq_animName.GetValue() + ".afanim",
		onComplete);
}

void AFGraphNode_PlaySequence::Evaluate(float deltaTime)
{
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
