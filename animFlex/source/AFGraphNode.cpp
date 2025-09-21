#include "AFGraphNode.h"

void AFGraphNode::Init()
{
}

void AFGraphNode::OnUpdate()
{
}

void AFGraphNode::PreEvaluate(float deltaTime)
{
}

void AFGraphNode::OnBecomeRelevant()
{
}

std::string AFGraphNode::GetNodeID() const
{
	return m_nodeId;
}

void AFGraphNode::AddSubNode(const std::string& subNodeId)
{
	auto it = std::find(m_subNodes.begin(), m_subNodes.end(), subNodeId);
	if (it == m_subNodes.end()) 
	{
		m_subNodes.push_back(subNodeId);
	}
}

std::vector<std::string> AFGraphNode::GetSubNodes() const
{
	return m_subNodes;
}

void AFGraphNode::SetNodeContext(const std::string& contextID)
{
	m_nodeContext = contextID;
}

const std::string& AFGraphNode::GetNodeContext() const
{
	return m_nodeContext;
}
