#include "AFStructs.h"
#include "AFAnimState.h"
#include "AFGame.h"
#include "AFGraphNode_State.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"
#include "AFStateClass.h"

void FAFBlendStack_Evaluator::Evaluate(float deltaTime, AFPose& pose, const FAFBlendStackEvalParams& params)
{
	std::shared_ptr<AFAnimState> animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
	if (!animState)
	{
		return;
	}

	if (!state && !clip)
	{
		return;
	}

	// Cache blend state IDs for debug.
	if (state)
	{
		nlohmann::json entry;
		entry["nodeId"] = state->GetNodeID();
		AFEvaluator::Get().AddLastActiveState(entry);

		bool stateTickCalled = false;
		AFStateClass* stateObj = state->GetStateObj().get();
		if (stateObj)
		{
			stateTickCalled = stateObj->CallFunctionByString(state->m_onTickFunStr.GetValue());
		}
		if (!stateTickCalled)
		{
			animState->CallFunctionByString(state->m_onTickFunStr.GetValue());
		}

		state->Evaluate(deltaTime);
		pose = state->GetGraph()->GetFinalPose();
	}
	// Support for blending separate clips.
	else if (clip)
	{
		pose.ApplyClip(clip, params.time, params.rootLock);
	}
}

void FAFBlendStack_Blender::Evaluate(float deltaTime, AFPose& pose, const FAFBlendStackEvalParams& params)
{
	if (AFMath::NearlyEqual(duration, 0.0f))
	{
		to->Evaluate(deltaTime, pose, params);
		return;
	}

	if (!from || !to)
	{
		return;
	}

	AFPose fromPose = {};
	from->Evaluate(deltaTime, fromPose, params);

	AFPose toPose = {};
	to->Evaluate(deltaTime, toPose, params);

	AFMath::BlendPoses(pose, fromPose, toPose, t / duration);
}

void FAFBlendStack_Blender::ProgressBlendTime(float deltaTime)
{
	t += direction == EAFBlendDirection::Forward ? deltaTime : -deltaTime;
	t = glm::clamp(t, 0.0f, duration);

	if (std::shared_ptr<FAFBlendStack_Blender> fromBlender = std::dynamic_pointer_cast<FAFBlendStack_Blender>(from))
	{
		fromBlender->ProgressBlendTime(deltaTime);
	}
}

bool FAFBlendStack_Blender::HasFinished() const
{
	return direction == EAFBlendDirection::Forward ? AFMath::NearlyEqual(t, duration) : AFMath::NearlyEqual(t, 0.0f);
}

float FAFBlendStack_Blender::GetBlendTime() const
{
	return t;
}
