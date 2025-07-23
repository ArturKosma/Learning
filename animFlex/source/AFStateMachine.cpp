#include "AFStateMachine.h"
#include "AFGraphNode.h"

void AFStateMachine::OnNodeCreated(const std::string& msg)
{
	printf("node created within state machine\n");

	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes creation.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];
	const std::string& nodeContext = node["nodeContext"];

	// Construct a node - it will be now accessible via m_idToNode hashmap.
	std::shared_ptr<AFGraphNode> newNode = AFGraphNodeRegistry::Get().CreateNode(nodeType, nodeId);
	newNode->m_nodeId = nodeId;
	newNode->Init();

	printf("node created with context: %s, type: %s\n", nodeContext.c_str(), nodeType.c_str());

	// #hack
	// Cache StateStart node, which will be the start of evaluation for the state machine graph.
	// @todo Don't check this upon every node creation.
	if (nodeType == "StateStart")
	{
		//printf("%s\n", "output pose created!");
		m_stateStartNode = std::dynamic_pointer_cast<AFGraphNode_StateStart>(newNode);
	}
}
