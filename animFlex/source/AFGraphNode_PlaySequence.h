#pragma once
#include <deque>
#include "AFGraphNode.h"
#include <string>
#include "AFPose.h"
#include "IAFSyncGroupInterface.h"

class AFGraphNode_PlaySequence : public AFGraphNodeCRTP<AFGraphNode_PlaySequence>, public IAFSyncGroupInterface
{
	AFCLASS(AFGraphNode_PlaySequence, "Play Sequence", "")

public:

	AFPARAM(std::string, playseq_animName, "", "", "Input", "HidePin|Dropdown_Anims|Show=playseq_showPin");
	AFPARAM(AFPose, playseq_outputPose, {}, "", "Output", "");
	AFPARAM(bool, playseq_forceRootLock, true, "Force Root Lock", "", "");
	AFPARAM(float, playseq_playrate, 1.0f, "Playrate", "", "");
	AFPARAM(bool, playseq_loop, true, "Loop", "", "");
	AFPARAM(bool, playseq_showStartTimePin, false, "Show Start Time Pin", "", "");
	AFPARAM(float, playseq_startTime, 0.0f, "Start Time", "Input", "HidePin|HideControl|Show=playseq_showStartTimePin");
	AFPARAM(float, playseq_endTime, -1.0f, "End Time", "", "");
	AFPARAM(bool, playseq_manualTime, false, "Should Use Manual Time", "", "");
	AFPARAM(float, playseq_manualTimeFloat, 0.0f, "Manual Time", "Input", "HidePin|HideControl|Show=playseq_manualTime");
	AFPARAM(float, playseq_blendTime, 0.0f, "Blend Time", "", "");
	AFPARAM(bool, playseq_showPin, false, "Show Pin", "", "");
	AFPARAM(std::string, playseq_syncGroupName, "", "Sync Group Name", "", "");
	AFPARAM(int, playseq_syncGroupMode, 0, "Sync Group Mode", "", "EAFSyncGroupMode_Enum");

	void OnUpdate() override;
	void Evaluate(float deltaTime) override;
	void OnBecomeRelevant() override;

	std::string GetSyncGroupName() override;
	EAFSyncGroupMode GetSyncGroupMode() override;
	float GetLocalTime() override;
	AFAnimationClip* GetAnimClip() override;

private:

	float m_localTime = 0.0f;
	std::string m_animName = "";
	std::shared_ptr<AFAnimationClip> m_animClip = nullptr;
	std::deque<std::shared_ptr<FAFBlendStack_Blender>> m_blendStack = {};
};
