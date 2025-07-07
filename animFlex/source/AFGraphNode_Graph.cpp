#include "AFGraphNode_Graph.h"

void AFGraphNode_Graph::Init()
{
	m_graph = std::make_shared<AFAnimGraph>();
}

void AFGraphNode_Graph::Evaluate(float deltaTime)
{
	m_graph->Evaluate(deltaTime);

	GraphOutput.SetValue(m_graph->GetFinalPose());
}

std::shared_ptr<AFAnimGraph> AFGraphNode_Graph::GetGraph() const
{
	return m_graph;
}
