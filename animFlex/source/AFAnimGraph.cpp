#include "AFAnimGraph.h"

#include "AFGraphNodeRegistry.h"
#include "third_party/json.hpp"

void AFAnimGraph::Compile(const std::string& graphString)
{
	printf("%s\n", "Compiling graph.");
	printf("%s\n", graphString.c_str());
}

void AFAnimGraph::Evaluate(float deltaTime)
{
}

void AFAnimGraph::OnNodeCreated(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes creation.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];

	GraphNodeConstructorFun ctor = AFGraphNodeRegistry::GRAPH_NODE_CONSTRUCTORS[nodeType];
	if (!ctor)
	{
		return;
	}

	// Construct a node - it will be now accessible via m_idToNode hashmap.
	m_idToNode[nodeId] = AFGraphNodeRegistry::GRAPH_NODE_CONSTRUCTORS[nodeType]();
}

void AFAnimGraph::OnNodeUpdated(const std::string& msg)
{
	printf("%s\n", msg.c_str());
}

void AFAnimGraph::OnNodeRemoved(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes removal.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];

	// Remove the node from hashmap and destroy the object.
	m_idToNode.erase(nodeId);
}
