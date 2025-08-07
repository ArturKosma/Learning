#pragma once
#include "AFGraphNode.h"
#include <string>

#include "AFPose.h"

class AFGraphNode_PlaySequence : public AFGraphNodeCRTP<AFGraphNode_PlaySequence>
{
	AFCLASS(AFGraphNode_PlaySequence, "Play Sequence", "");

public:

	AFPARAM(std::string, playseq_animName, "", "", "Input", "HidePin|Dropdown_Anims");
	AFPARAM(AFPose, playseq_outputPose, "", "", "Output", "");
	AFPARAM(bool, playseq_useRootmotion, false, "Use Rootmotion", "", "");
	AFPARAM(float, playseq_playrate, 1.0f, "Playrate", "", "");

	void Init() override;
	void OnUpdate() override;
	void Evaluate(float deltaTime) override;

private:

	float m_localTime = 0.0f;
	std::shared_ptr<AFAnimationClip> m_animClip = nullptr;

};
