#pragma once
#include "AFGraphNode.h"
#include "AFStateMachine.h"

class AFGraphNode_StateMachine : public AFGraphNodeCRTP<AFGraphNode_StateMachine>
{
	AFCLASS(AFGraphNode_StateMachine, "State Machine", "")

public:

	AFPARAM(AFPose, StateMachineOutput, {}, "", "Output", "");
	AFPARAM(float, m_maxTransitionsPerTick, 5.0, "Max Transitions Per Tick", "", ""); // @todo Enable ints in frontend.

	void Init() override;
	void OnUpdate() override;
	void PreEvaluate(float deltaTime) override;
	void Evaluate(float deltaTime) override;
	void OnBecomeRelevant() override;
	std::shared_ptr<AFStateMachine> GetStateMachine() const;

protected:

	std::shared_ptr<AFStateMachine> m_stateMachine = nullptr;
};
