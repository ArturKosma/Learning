#include "AFGraphNode_StateMachine.h"

void AFGraphNode_StateMachine::Init()
{
	m_stateMachine = std::make_shared<AFStateMachine>();
}

void AFGraphNode_StateMachine::Evaluate(float deltaTime)
{
	m_stateMachine->Evaluate(deltaTime);

	StateMachineOutput.SetValue(m_stateMachine->GetFinalPose());
}

std::shared_ptr<AFStateMachine> AFGraphNode_StateMachine::GetStateMachine() const
{
	return m_stateMachine;
}
