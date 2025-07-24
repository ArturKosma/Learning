#include "AFStateMachine.h"
#include "AFGraphNode.h"
#include "AFGraphNode_StateCond.h"

void AFStateMachine::Evaluate(float deltaTime)
{
	// Erase expired states (deleted nodes).
	m_states.erase(std::remove_if(m_states.begin(), m_states.end(),
		[](const std::weak_ptr<AFGraphNode>& state)
		{
			return state.expired();
		}), m_states.end());

	// Check currently active state.
	// The previously active node might've got removed, in that case we start over.
	if (m_currentState.expired())
	{
		m_currentState = m_stateStartNode;
	}

	// Container for the new state to transition to.
	std::weak_ptr<AFGraphNode> nextState = m_currentState;

	// Max 5 jumps per evaluate.
	for (int i = 0; i < 5; i++)
	{
		if (nextState.expired())
		{
			// Something went very wrong.
			// nextState was a node specified by one of the TO transitions, but that node doesn't exist in the node registry.
			return;
		}

		// Find all outgoing connections.
		std::vector<FAFStateConnection> outgoing = {};
		std::copy_if(m_connections.begin(), m_connections.end(), std::back_inserter(outgoing),
			[this, nextState](const FAFStateConnection& conn) {return conn.from == nextState.lock()->GetNodeID(); });

		if (outgoing.empty())
		{
			break; // No transition found.
		}

		auto foundTransition = std::find_if(outgoing.begin(), outgoing.end(), [deltaTime](const FAFStateConnection& conn)
			{
				// No conditional node. It's an always true transition condition.
				if (conn.cond.empty())
				{
					return true;
				}

				std::shared_ptr<AFGraphNode_StateCond> cond = std::dynamic_pointer_cast<AFGraphNode_StateCond>(AFGraphNodeRegistry::Get().GetNode(conn.cond));
				if (cond)
				{
					// Conditional node exists. Check its condition.
					cond->Evaluate(deltaTime);
					return cond->GetLastEvaluation();
				}

				return false;
			});

		if (foundTransition == outgoing.end())
		{
			break; // No transition found.
		}

		// There is a transition. Cache the target node and try jumping again.
		nextState = AFGraphNodeRegistry::Get().GetNode(foundTransition->to);
	}

	// There was no transition at all, evaluate our current state.
	if (nextState.lock() == m_currentState.lock())
	{
		printf("evaluating id: %s\n", m_currentState.lock()->GetNodeID().c_str());
		m_currentState.lock()->Evaluate(deltaTime);
		return;
	}

	// There was a transition. Simply jump there and evaluate.
	// @todo Blending.
	printf("next state id: %s\n", nextState.lock()->GetNodeID().c_str());
	m_currentState = nextState;
	m_currentState.lock()->Evaluate(deltaTime);
}

void AFStateMachine::OnNodeCreated(const std::string& msg)
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
	newNode->Init();

	// #hack
	// Cache StateStart node, which will be the start of evaluation for the state machine graph.
	// @todo Don't check this upon every node creation.
	if (nodeType == "StateStart")
	{
		m_stateStartNode = std::dynamic_pointer_cast<AFGraphNode_StateStart>(newNode);
	}

	// We hold weak pointers in m_states so the shared pointer will be implicitly casted to it.
	// Weaks are to avoid creating a whole observer pattern for now.
	// Once the node is removed I will know about it via weak.
	if (AFGraphNode::IsAnyOf<AFGraphNode_State, AFGraphNode_StateStart>(newNode))
	{
		std::weak_ptr<AFGraphNode> weakState = std::dynamic_pointer_cast<AFGraphNode>(newNode);
		m_states.push_back(weakState);
		printf("created: %s\n", nodeId.c_str());
	}
}

void AFStateMachine::OnConnectionCreated(const std::string& msg)
{
	nlohmann::json connections = nlohmann::json::parse(msg);
	const auto& connection = connections[0];

	const std::string& nodeFromID = connection["nodeFromID"];
	const std::string& nodeToID = connection["nodeToID"];
	const std::string& connectionID = connection["connectionID"];
	const std::string& nodeCondID = connection["nodeCondID"];

	// If condition is non empty, there has to be a node for it.
	if (!nodeCondID.empty())
	{
		std::shared_ptr<AFGraphNode> condNode = AFGraphNodeRegistry::Get().GetNode(nodeCondID);
		if (!condNode || !AFGraphNode::IsAnyOf<AFGraphNode_StateCond>(condNode))
		{
			return;
		}
	}

	// Sanity.
	if (nodeFromID.empty() || nodeToID.empty() || connectionID.empty() || nodeFromID == nodeToID)
	{
		return;
	}

	// Check if we don't have this exact connection yet.
	// Frontend sometimes gets confused and sends multiple signals.
	auto found = std::find_if(m_connections.begin(), m_connections.end(),
		[connectionID](const FAFStateConnection& conn)
		{
			return conn.id == connectionID;
		});
	if (found != m_connections.end())
	{
		return;
	}

	// Register a new connection between states.
	FAFStateConnection newStateConnection = {};
	newStateConnection.from = nodeFromID;
	newStateConnection.to = nodeToID;
	newStateConnection.id = connectionID;
	newStateConnection.cond = nodeCondID; // Empty cond means it's a connection from entry state and thus it's always true.
	m_connections.push_back(newStateConnection);
}

void AFStateMachine::OnConnectionRemoved(const std::string& msg)
{
	nlohmann::json connections = nlohmann::json::parse(msg);
	const auto& connection = connections[0];

	const std::string& connectionID = connection["connectionID"];

	// Sanity.
	if (connectionID.empty())
	{
		return;
	}

	m_connections.erase(std::remove_if(m_connections.begin(), m_connections.end(),
		[connectionID](const FAFStateConnection& conn)
		{
			return conn.id == connectionID;
		}), m_connections.end());
}
