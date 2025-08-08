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

	// Increase time & sample anim.
	m_localTime += deltaTime * playseq_playrate;
	const float time = std::fmod(m_localTime, m_animClip->GetClipEndTime());
	const_cast<AFPose&>(playseq_outputPose.GetValue()).ApplyClip(m_animClip, time, playseq_forceRootLock);
}
