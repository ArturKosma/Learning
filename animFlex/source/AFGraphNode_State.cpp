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

void AFGraphNode_State::OnBecomeRelevant()
{
	// Call reset on all sub-nodes.
	for (const std::string& id : m_subNodes)
	{
		std::shared_ptr<AFGraphNode> subNode = AFGraphNodeRegistry::Get().GetNode(id);
		if (subNode)
		{
			subNode->OnBecomeRelevant();
		}
	}
}

std::shared_ptr<AFAnimGraph> AFGraphNode_State::GetGraph() const
{
	return m_graph;
}
