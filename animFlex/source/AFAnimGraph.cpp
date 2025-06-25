#include "AFAnimGraph.h"

#include "AFEvaluator.h"
#include "AFGraphNode.h"
#include "AFGraphNodeRegistry.h"
#include "AFGraphNode_OutputPose.h"

#include "third_party/json.hpp"

nlohmann::json AFAnimGraph::lastActiveSockets = nlohmann::json::array();

void AFAnimGraph::Compile(const std::string& graphString)
{
	printf("%s\n", "Compiling graph.");
	printf("%s\n", graphString.c_str());
}

void AFAnimGraph::Evaluate(float deltaTime)
{
	if (m_outputPoseNode)
	{
		// Mark all nodes dirty. This will allow for new evaluation of each of them.
		AFEvaluator::Get().ClearEvaluationState();

		std::string connectedNodeId = "";
		std::string connectedSocketName = "";
		m_outputPoseNode->Pose.GetConnection(connectedNodeId, connectedSocketName);
		if (connectedNodeId.empty() || connectedSocketName.empty())
		{
			return;
		}

		std::shared_ptr<AFGraphNode> connectedNode = AFGraphNodeRegistry::Get().GetNode(connectedNodeId);
		if (!connectedNode)
		{
			return;
		}

		// Evaluate the connected node.
		AFEvaluator::Get().EvaluateNode(connectedNode);

		// Go through the static properties in order to find value of the connected socket.
		std::vector<std::shared_ptr<FAFParamStaticPropertyBase>> staticProperties = AFGraphNodeRegistry::Get().GetStaticProperties(connectedNode->GetNodeType());
		for (std::shared_ptr<FAFParamStaticPropertyBase> property : staticProperties)
		{
			const std::string& paramName = property->GetParamName();
			if (paramName == connectedSocketName)
			{
				m_finalPose = *static_cast<FAFParam<AFPose>*>(property->GetParam(connectedNode));
			}
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
	newNode->Init();

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
	std::unordered_map<std::string, nlohmann::json> paramToValueField;
	for (const auto& socket : node["sockets"])
	{
		std::string varName = socket["var_name"];
		const nlohmann::json& valueField = socket["valueField"];
		paramToValueField[varName] = valueField;
	}

	// Apply the value field from JSON to every param.
	std::vector<std::shared_ptr<FAFParamStaticPropertyBase>> staticProperties = AFGraphNodeRegistry::Get().GetStaticProperties(nodeType);
	for (std::shared_ptr<FAFParamStaticPropertyBase> property : staticProperties)
	{
		const std::string& paramName = property->GetParamName();

		if (paramToValueField.count(paramName))
		{
			property->Apply(editedNode, paramToValueField[paramName]);
		}
	}

	// Inform the node instance it was edited.
	editedNode->OnUpdate();
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

const AFPose& AFAnimGraph::GetFinalPose() const
{
	return m_finalPose;
}

void AFAnimGraph::AddLastActiveSocket(const nlohmann::json& newSocket)
{
	lastActiveSockets.push_back(newSocket);
}

const std::string& AFAnimGraph::GetLastActiveSockets()
{
	static std::string json = lastActiveSockets.dump();
	return json;
}

void AFAnimGraph::ClearLastActiveSockets()
{
	lastActiveSockets = nlohmann::json::array();
}
