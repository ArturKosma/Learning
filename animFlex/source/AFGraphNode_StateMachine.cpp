#include "AFGraphNode_StateMachine.h"

void AFGraphNode_StateMachine::Init()
{
	m_stateMachine = std::make_shared<AFStateMachine>();
}

void AFGraphNode_StateMachine::OnUpdate()
{
	m_stateMachine->m_maxTransitionsPerTick = static_cast<int>(glm::clamp(m_maxTransitionsPerTick.GetValue(), 0.0f, 10.0f));
}

void AFGraphNode_StateMachine::PreEvaluate(float deltaTime)
{
	m_stateMachine->PreEvaluate(deltaTime);
}

void AFGraphNode_StateMachine::Evaluate(float deltaTime)
{
	m_stateMachine->Evaluate(deltaTime);

	StateMachineOutput.SetValue(m_stateMachine->GetFinalPose());
}

void AFGraphNode_StateMachine::OnBecomeRelevant()
{
	m_stateMachine->Reset();
}

std::shared_ptr<AFStateMachine> AFGraphNode_StateMachine::GetStateMachine() const
{
	return m_stateMachine;
}
