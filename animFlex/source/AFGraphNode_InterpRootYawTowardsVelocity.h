#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_InterpRootYawTowardsVelocity : public AFGraphNodeCRTP<AFGraphNode_InterpRootYawTowardsVelocity>
{
	AFCLASS(AFGraphNode_InterpRootYawTowardsVelocity, "Interp Root Yaw Towards Velocity", "")

public:

	AFPARAM(AFPose, m_inputPose, {}, "", "Input", "");
	AFPARAM(float, m_rate, 250.0f, "Rate", "Input", "");
	AFPARAM(bool, m_authority, false, "Yaw Authority", "Input", "");
	AFPARAM(AFPose, m_outputPose, {}, "", "Output", "");

	void Init() override;
	void Evaluate(float deltaTime) override;

private:

	std::shared_ptr<class AFAnimState> m_animState = nullptr;
};
