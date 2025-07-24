#include "AFGraphNode_StateCond.h"

void AFGraphNode_StateCond::Init()
{
	m_graph = std::make_shared<AFAnimGraph>();
}

void AFGraphNode_StateCond::Evaluate(float deltaTime)
{
}

std::shared_ptr<AFAnimGraph> AFGraphNode_StateCond::GetGraph() const
{
	return m_graph;
}

bool AFGraphNode_StateCond::GetLastEvaluation() const
{
	return m_lastEval;
}
