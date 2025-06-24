#include "AFGraphNode_PlaySequence.h"

#include "AFContent.h"
#include "AFMesh.h"

void AFGraphNode_PlaySequence::Init()
{
	// #hack.
	// In normal situation this would fetch the skeleton that the anim blueprint was created for.
	std::shared_ptr<AFMesh> mannequinMesh = AFContent::Get().FindAsset<AFMesh>("sk_mannequin");
	const_cast<AFPose&>(outputPose.GetValue()).ApplyJoints(mannequinMesh->GetJoints());
}

void AFGraphNode_PlaySequence::OnUpdate()
{
	auto onComplete = [this](std::shared_ptr<AFAnimationClip> fetchedAnim)
		{
			m_animClip = fetchedAnim;
			m_localTime = 0.0f;
		};

	AFContent::Get().FetchAsset<AFAnimationClip>("https://cdn.jsdelivr.net/gh/ArturKosma/assets@main/anims/", 
		animName.GetValue().c_str(),
		onComplete);
}

void AFGraphNode_PlaySequence::Evaluate(float deltaTime)
{
	if(!m_animClip)
	{
		return;
	}

	// Increase time & sample anim.
	m_localTime += deltaTime;
	const float time = std::fmod(m_localTime, m_animClip->GetClipEndTime());
	const_cast<AFPose&>(outputPose.GetValue()).ApplyClip(m_animClip, time);
}
