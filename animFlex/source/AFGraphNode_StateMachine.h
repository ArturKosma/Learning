#pragma once
#include "AFGraphNode.h"
#include "AFStateMachine.h"

class AFGraphNode_StateMachine : public AFGraphNodeCRTP<AFGraphNode_StateMachine>
{
	AFCLASS(AFGraphNode_StateMachine, "State Machine", "")

public:

	AFPARAM(AFPose, StateMachineOutput, "", "", "Output", "");

	void Init() override;

	void Evaluate(float deltaTime) override;
	std::shared_ptr<AFStateMachine> GetStateMachine() const;

protected:

	std::shared_ptr<AFStateMachine> m_stateMachine = nullptr;
};
