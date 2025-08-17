#include "AFAnimGraphCond.h"

void AFAnimGraphCond::Evaluate(float deltaTime)
{
	if (m_outputCondNode)
	{
		// Get value for the output cond node.
		// This should trigger whole chain of node evaluations.
		m_finalCond = m_outputCondNode->OutputCond.GetValue();
	}
}

void AFAnimGraphCond::OnNodeCreated(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes creation.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];
	const std::string& nodeContext = node["nodeContext"];

	// Construct a node - it will be now accessible via m_idToNode hashmap.
	std::shared_ptr<AFGraphNode> newNode = AFGraphNodeRegistry::Get().CreateNode(nodeType, nodeId);
	newNode->m_nodeId = nodeId;
	newNode->m_nodeContext = nodeContext;
	newNode->Init();

	// #hack
	// Cache OutputCond node, which will be the start of evaluation for the whole graph.
	// @todo Don't check this upon every node creation.
	if (nodeType == "OutputCond")
	{
		m_outputCondNode = std::dynamic_pointer_cast<AFGraphNode_OutputCond>(newNode);
	}
}

bool AFAnimGraphCond::GetEvalResult() const
{
	return m_finalCond;
}
