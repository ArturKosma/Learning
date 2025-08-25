#pragma once
#include "AFGraphNode.h"
#include <string>

#include "AFFloatCurve.h"
#include "AFPose.h"

class AFGraphNode_PlaySequence : public AFGraphNodeCRTP<AFGraphNode_PlaySequence>
{
	AFCLASS(AFGraphNode_PlaySequence, "Play Sequence", "")

public:

	AFPARAM(std::string, playseq_animName, "", "", "Input", "HidePin|Dropdown_Anims|Show=playseq_showPin");
	AFPARAM(AFPose, playseq_outputPose, {}, "", "Output", "");
	AFPARAM(bool, playseq_forceRootLock, true, "Force Root Lock", "", "");
	AFPARAM(float, playseq_playrate, 1.0f, "Playrate", "", "");
	AFPARAM(bool, playseq_loop, true, "Loop", "", "");
	AFPARAM(float, playseq_startTime, 0.0f, "Start Time", "", "");
	AFPARAM(float, playseq_endTime, -1.0f, "End Time", "", "");
	AFPARAM(bool, playseq_manualTime, false, "Should Use Manual Time", "", "");
	AFPARAM(float, playseq_manualTimeFloat, 0.0f, "Manual Time", "Input", "HidePin|HideControl|Show=playseq_manualTime");
	AFPARAM(bool, playseq_showPin, false, "Show Pin", "", "");

	void OnUpdate() override;
	void Evaluate(float deltaTime) override;
	void OnReset() override;

private:

	float m_localTime = 0.0f;
	std::shared_ptr<AFAnimationClip> m_animClip = nullptr;
};
