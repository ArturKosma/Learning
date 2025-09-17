#include "AFStateMachine.h"
#include "AFAnimState.h"
#include "AFGame.h"
#include "AFGraphNode.h"
#include "AFGraphNode_StateCond.h"
#include "AFGraphNode_State.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

void AFStateMachine::PreEvaluate(float deltaTime)
{
	
}

void AFStateMachine::Evaluate(float deltaTime)
{
	std::shared_ptr<AFAnimState> animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();
	if (!animState)
	{
		return;
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

	// Blend time, taken from the conditional node.
	float blendTime = 0.15f;

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

		// Update blend time.
		std::shared_ptr<AFGraphNode_StateCond> nextStateCond = std::dynamic_pointer_cast<AFGraphNode_StateCond>(AFGraphNodeRegistry::Get().GetNode(foundTransition->cond));
		if (nextStateCond)
		{
			blendTime = nextStateCond->m_blendTime.GetValue();
		}
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
		bool reverseTransition = false;

		// Jump there if we were in the entry state.
		if (std::dynamic_pointer_cast<AFGraphNode_StateStart>(m_currentState.lock()))
		{
			m_currentState = nextState;
		}
		// Schedule the blend.
		else
		{
			reverseTransition = ScheduleBlend(m_currentState.lock()->GetNodeID(), nextState.lock()->GetNodeID(), blendTime);
			m_currentState = nextState;
		}

		// Call OnEnter function on the new state.
		// Reverse transition so A->B turning B->A does not trigger new enter fun.
		std::shared_ptr<AFGraphNode_State> state = std::dynamic_pointer_cast<AFGraphNode_State>(m_currentState.lock());
		if (state && !reverseTransition)
		{
			const std::string& funStr = state->m_onEnterFunStr.GetValue();
			animState->CallFunctionByString(funStr);
		}
	}

	// Evaluate final pose from blend stack.
	if (!m_blendStack.empty())
	{
		FAFBlendStack_Blender* topBlender = m_blendStack.back().get();
		if (!topBlender)
		{
			return;
		}

		// Progress blend time.
		topBlender->ProgressBlendTime(deltaTime);

		// Evaluate top of the blend stack.
		// Each blender accesses previous blender, ultimately ending at simple evaluators.
		topBlender->Evaluate(deltaTime, m_finalPose);

		// If top blender is done blending, we drop whole stack.
		if (topBlender->HasFinished())
		{
			m_blendStack.clear();
		}

	}
	// Evaluate final pose from clear state.
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

		animState->CallFunctionByString(currentState->m_onTickFunStr.GetValue());
		currentState->Evaluate(deltaTime);

		// Cache final pose.
		m_finalPose.CopyPoseFrom(currentState->GetGraph()->GetFinalPose());
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

bool AFStateMachine::ScheduleBlend(const std::string& blendFrom, const std::string& blendTo, float blendLength)
{
	if (blendLength <= 0.0f)
	{
		return false;
	}

	if (blendFrom.empty() || blendTo.empty())
	{
		return false;
	}

	std::shared_ptr<AFGraphNode_State> blendFromState = std::dynamic_pointer_cast<AFGraphNode_State>(AFGraphNodeRegistry::Get().GetNode(blendFrom));
	std::shared_ptr<AFGraphNode_State> blendToState = std::dynamic_pointer_cast<AFGraphNode_State>(AFGraphNodeRegistry::Get().GetNode(blendTo));

	if (!blendFromState || !blendToState)
	{
		return false;
	}

	// If we are currently blending A->B, and we want to blend back B->A or vice versa, we simply change blend direction.
	if (!m_blendStack.empty())
	{
		FAFBlendStack_Blender* top = m_blendStack.back().get();
		if (!top)
		{
			return false;
		}

		std::shared_ptr<AFGraphNode_State> stackTopFromState = nullptr;
		std::shared_ptr<AFGraphNode_State> stackTopToState = top->to->state;

		// If A is a blender.
		if (std::shared_ptr<FAFBlendStack_Blender> topFromBlender = std::dynamic_pointer_cast<FAFBlendStack_Blender>(top->from))
		{
			stackTopFromState = topFromBlender->to->state;
			
		}
		// A is a simple evaluator.
		else if (std::shared_ptr<FAFBlendStack_Evaluator> topFromEvaluator = std::dynamic_pointer_cast<FAFBlendStack_Evaluator>(top->from))
		{
			stackTopFromState = topFromEvaluator->state;
		}

		// If a->b forward is happening and we want b->a.
		const bool btoa = (stackTopFromState.get() == blendToState.get()) && (stackTopToState.get() == blendFromState.get());
		if (btoa)
		{
			top->direction = EAFBlendDirection::Backward;
			return true;
		}

		// If a->b backward is happening and we want a->b forward.
		const bool atob = (stackTopFromState.get() == blendFromState.get()) && (stackTopToState.get() == blendToState.get());
		if (atob && top->direction == EAFBlendDirection::Backward)
		{
			top->direction = EAFBlendDirection::Forward;
			return true;
		}

		// @todo Support the different blend time when direction change occurs.
	}

	// Potential evaluators.
	std::shared_ptr<FAFBlendStack_Evaluator> fromEvaluator = std::make_shared<FAFBlendStack_Evaluator>();
	fromEvaluator->state = blendFromState;
	std::shared_ptr<FAFBlendStack_Evaluator> toEvaluator = std::make_shared<FAFBlendStack_Evaluator>();
	toEvaluator->state = blendToState;

	// Create new blend which will end up on the top of the stack.
	std::shared_ptr<FAFBlendStack_Blender> newBlender = std::make_shared<FAFBlendStack_Blender>();
	newBlender->duration = blendLength;
	newBlender->t = 0.0f;

	// From is either simple evaluator of previous top of the blend stack.
	std::shared_ptr<IAFBlendStack_Node> fromBlendOrEval = m_blendStack.empty() ?
	std::static_pointer_cast<IAFBlendStack_Node>(fromEvaluator) :
	std::static_pointer_cast<IAFBlendStack_Node>(m_blendStack.back());
	newBlender->from = fromBlendOrEval;
	newBlender->to = toEvaluator;
	newBlender->direction = EAFBlendDirection::Forward;
	m_blendStack.push_back(newBlender);

	return false;
}
