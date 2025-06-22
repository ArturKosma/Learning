#include "AFAnimGraph.h"
#include "AFGraphNode.h"
#include "AFGraphNodeRegistry.h"
#include "AFGraphNode_OutputPose.h"

#include "third_party/json.hpp"

void AFAnimGraph::Compile(const std::string& graphString)
{
	printf("%s\n", "Compiling graph.");
	printf("%s\n", graphString.c_str());
}

void AFAnimGraph::Evaluate(float deltaTime)
{
	if (m_outputPoseNode)
	{
		const std::string& connectedNodeId = m_outputPoseNode->Pose.connectedNodeId;
		if (connectedNodeId != "")
		{
			std::shared_ptr<AFGraphNode> connectedNode = AFGraphNodeRegistry::Get().GetNode(connectedNodeId);
			if (!connectedNode)
			{
				return;
			}

			connectedNode->Evaluate(deltaTime);
		}
		else
		{
			printf("output pose not connected :(\n");
		}
	}
}

void AFAnimGraph::OnNodeCreated(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes creation.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];

	// Construct a node - it will be now accessible via m_idToNode hashmap.
	std::shared_ptr<AFGraphNode> newNode = AFGraphNodeRegistry::Get().CreateNode(nodeType, nodeId);

	// #hack
	// Cache OutputPose node, which will be the start of evaluation for the whole graph.
	// @todo Don't check this upon every node creation.
	if (nodeType == "OutputPose")
	{
		m_outputPoseNode = std::dynamic_pointer_cast<AFGraphNode_OutputPose>(newNode);
	}
}

void AFAnimGraph::OnNodeUpdated(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes edition.
	const auto& node = nodes[0];

	// Fetch edited node ID.
	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];

	// Find the node to edit.
	std::shared_ptr<AFGraphNode> editedNode = AFGraphNodeRegistry::Get().GetNode(nodeId);
	if (!editedNode)
	{
		return;
	}

	// Get info from the JSON for every param, and keep it as a varName to JSON map.
	std::unordered_map<std::string, std::string> paramToMsg = {};
	for (const auto& socket : node["sockets"])
	{
		std::string varName = socket["var_name"];
		std::string valueStr = ReadJSONSocketValue(socket);
		paramToMsg[varName] = valueStr;
	}

	// Apply the value field from JSON to every param.
	std::vector<std::shared_ptr<FAFParamStaticPropertyBase>> staticProperties = AFGraphNodeRegistry::Get().GetStaticProperties(nodeType);
	for (std::shared_ptr<FAFParamStaticPropertyBase> property : staticProperties)
	{
		// Pass the JSON message to the node. Node itself will then handle the incoming string and update its value or connection.
		property->Apply(editedNode, paramToMsg[property->GetParamName()]);
	}
}

void AFAnimGraph::OnNodeRemoved(const std::string& msg)
{
	nlohmann::json nodes = nlohmann::json::parse(msg);

	// @todo Enable multiple nodes removal.
	const auto& node = nodes[0];

	const std::string& nodeId = node["nodeId"];
	const std::string& nodeType = node["nodeType"];

	// Remove the node from hashmap and destroy the object.
	AFGraphNodeRegistry::Get().RemoveNode(nodeId);
}

std::string AFAnimGraph::ReadJSONSocketValue(const nlohmann::json& socket)
{
	const nlohmann::json& valueField = socket["valueField"];
	std::string valueStr = "";

	if (valueField.contains("value"))
	{
		std::string value = valueField["value"].get<std::string>();
		valueStr = "value:" + value;
	}

	if (valueField.contains("connectedNodeId"))
	{
		std::string nodeID = valueField["connectedNodeId"].get<std::string>();
		std::string socketName = valueField["connectedSocketName"].get<std::string>();

		valueStr = "connection:" + nodeID + "/" + socketName;
	}

	return valueStr;
}
