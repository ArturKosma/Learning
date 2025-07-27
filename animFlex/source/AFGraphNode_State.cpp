#include "AFGraphNode_State.h"
#include "AFAnimGraph.h"

void AFGraphNode_State::Init()
{
	m_graph = std::make_shared<AFAnimGraph>();
}

void AFGraphNode_State::Evaluate(float deltaTime)
{
	m_graph->Evaluate(deltaTime);
}

std::shared_ptr<AFAnimGraph> AFGraphNode_State::GetGraph() const
{
	return m_graph;
}
