#include "AFStateMachine.h"
#include "AFGraphNode.h"
#include "AFGraphNode_StateCond.h"
#include "AFMath.h"

void AFStateMachine::Evaluate(float deltaTime)
{
	// Progress blend.
	if (m_isBlending)
	{
		m_currentBlend.currentBlendTime = glm::clamp(m_currentBlend.currentBlendTime + deltaTime, 0.0f, m_currentBlend.blendLength);
	}

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

	if (nextState.expired())
	{
		// Something went very wrong.
		// nextState was a node specified by one of the TO transitions, but that node doesn't exist in the node registry.
		return;
	}

	// There was no transition at all, evaluate our current state.
	if (nextState.lock() == m_currentState.lock())
	{
		
	}
	else
	{
		// There was a transition.
		//printf("next state id: %s\n", nextState.lock()->GetNodeID().c_str());

		// Jump there if we were in the entry state.
		if (std::dynamic_pointer_cast<AFGraphNode_StateStart>(m_currentState.lock()))
		{
			m_currentState = nextState;
		}
		// Schedule the blend.
		else
		{
			ScheduleBlend(m_currentState.lock()->GetNodeID(), nextState.lock()->GetNodeID(), 0.25f);
			m_currentState = nextState;
		}
	}

	// Evaluate final pose.
	if (m_isBlending)
	{
		// Evaluate both blending states.
		std::shared_ptr<AFGraphNode_State> from = m_currentBlend.blendFrom.lock();
		std::shared_ptr<AFGraphNode_State> to = m_currentBlend.blendTo.lock();

		// If any of the blending states became invalid, stop the blend.
		if (!from || !to)
		{
			ClearBlend();
		}

		// Cache blend states IDs for debug.
		// @todo Show blend progress as debug.
		nlohmann::json fromEntry;
		nlohmann::json toEntry;
		fromEntry["nodeId"] = from->GetNodeID();
		toEntry["nodeId"] = to->GetNodeID();
		AFEvaluator::Get().AddLastActiveState(fromEntry);
		AFEvaluator::Get().AddLastActiveState(toEntry);

		from->Evaluate(deltaTime);
		to->Evaluate(deltaTime);

		// Blend poses.
		AFMath::BlendPoses(m_finalPose, from->GetGraph()->GetFinalPose(), to->GetGraph()->GetFinalPose(), m_currentBlend.currentBlendTime / m_currentBlend.blendLength);

		// If we reached max blend time carry on blend finish.
		if (AFMath::NearlyEqual(m_currentBlend.currentBlendTime, m_currentBlend.blendLength))
		{
			ClearBlend();
		}
	}
	else
	{
		std::shared_ptr<AFGraphNode_State> currentState = std::dynamic_pointer_cast<AFGraphNode_State>(m_currentState.lock());
		if (!currentState)
		{
			return;
		}

		// Cache last active state node ID.
		// Used for debug.
		nlohmann::json lastActiveEntry;
		lastActiveEntry["nodeId"] = currentState->GetNodeID();
		AFEvaluator::Get().AddLastActiveState(lastActiveEntry);

		currentState->Evaluate(deltaTime);

		// Cache final pose.
		m_finalPose.CopyTransformsFrom(currentState->GetGraph()->GetFinalPose());
	}
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
	newNode->m_nodeContext = nodeContext;
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
		//printf("created: %s\n", nodeId.c_str());
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

void AFStateMachine::ScheduleBlend(const std::string& blendFrom, const std::string& blendTo, float blendLength)
{
	if (blendLength <= 0.0f)
	{
		return;
	}

	if (blendFrom.empty() || blendTo.empty())
	{
		return;
	}

	std::weak_ptr<AFGraphNode_State> blendFromNode = std::dynamic_pointer_cast<AFGraphNode_State>(AFGraphNodeRegistry::Get().GetNode(blendFrom));
	std::weak_ptr<AFGraphNode_State> blendToNode = std::dynamic_pointer_cast<AFGraphNode_State>(AFGraphNodeRegistry::Get().GetNode(blendTo));

	if (!blendFromNode.lock() || !blendToNode.lock())
	{
		return;
	}

	FAFStateBlend newBlend;
	newBlend.blendLength = blendLength;
	newBlend.currentBlendTime = 0.0f;

	// If the new blend is just a reverse of a previous blend (to-from are swapped) we start the blend from where it ended.
	// @todo Treat blend as a state, and blend from a state.
	if (m_isBlending && (blendFrom == m_currentBlend.blendTo.lock()->GetNodeID() && blendTo == m_currentBlend.blendFrom.lock()->GetNodeID()))
	{
		const float normalizedOldProgress = m_currentBlend.currentBlendTime / m_currentBlend.blendLength;
		newBlend.currentBlendTime = m_currentBlend.blendLength - (normalizedOldProgress * m_currentBlend.blendLength);
	}

	newBlend.blendFrom = blendFromNode;
	newBlend.blendTo = blendToNode;

	m_currentBlend = newBlend;
	m_isBlending = true;
}

void AFStateMachine::ClearBlend()
{
	m_currentBlend = FAFStateBlend();
	m_isBlending = false;
}
