#include "AFEvaluator.h"
#include "AFGraphNode.h"
#include "AFTimerManager.h"
#include "IAFSyncGroupInterface.h"

void AFEvaluator::EvaluateNode(std::shared_ptr<AFGraphNode> node)
{
	if (!node)
	{
		return;
	}

	auto it = std::find(m_evaluated.begin(), m_evaluated.end(), node);
	if (it == m_evaluated.end())
	{
		// Node wasn't evaluated this turn. Evaluate it.
		const float deltaTime = AFTimerManager::GetDeltaTime();
		node->Evaluate(m_animPaused ? 0.0f : deltaTime * m_animPlayrate);

		m_evaluated.push_back(node);
	}
}

void AFEvaluator::PreEvaluateNode(std::shared_ptr<class AFGraphNode> node)
{
	// Early return if invalid node.
	if (!node)
	{
		return;
	}

	// Pre evaluate only once.
	auto it = std::find(m_preevaluated.begin(), m_preevaluated.end(), node);
	if (it != m_preevaluated.end())
	{
		return;
	}

	// Remember this node was pre evaluated.
	m_preevaluated.push_back(node);

	// Get connection per property.
	std::vector<std::shared_ptr<FAFParamStaticPropertyBase>> staticProperties = AFGraphNodeRegistry::Get().GetStaticProperties(node->GetNodeType());
	for (std::shared_ptr<FAFParamStaticPropertyBase> property : staticProperties)
	{
		if (!property->GetIsInput())
		{
			continue;
		}

		FAFParamBase* param = static_cast<FAFParamBase*>(property->GetParam(node));
		if (!param)
		{
			continue;
		}

		std::string connectedNodeId;
		std::string connectedSocketName;
		const bool connected = param->GetConnection(connectedNodeId, connectedSocketName);
		if (!connected)
		{
			continue;
		}

		std::shared_ptr<AFGraphNode> connectedNode = AFGraphNodeRegistry::Get().GetNode(connectedNodeId);
		if (!connectedNode)
		{
			continue;
		}

		// Recurrence down the chain.
		PreEvaluateNode(connectedNode);
	}

	// Pre evaluate subnodes.
	for (const std::string& subNodeId : node->GetSubNodes())
	{
		std::shared_ptr<AFGraphNode> subNode = AFGraphNodeRegistry::Get().GetNode(subNodeId);
		if (!subNode)
		{
			continue;
		}

		PreEvaluateNode(subNode);
	}

	// Pre evaluate at the very end, so that the furthest nodes down the chain get pre evaluated first.
	const float deltaTime = AFTimerManager::GetDeltaTime();
	node->PreEvaluate(m_animPaused ? 0.0f : deltaTime * m_animPlayrate);
}

void AFEvaluator::AddLastActiveSocket(const nlohmann::json& newSocket)
{
	m_lastActiveSockets.push_back(newSocket);
}

std::string AFEvaluator::GetLastActiveSockets()
{
	return m_lastActiveSockets.dump();
}

void AFEvaluator::ClearLastActiveSockets()
{
	m_lastActiveSockets = nlohmann::json::array();
}

void AFEvaluator::AddLastActiveState(const nlohmann::json& newState)
{
	auto it = std::find_if(
		m_lastActiveStatesCached.begin(),
		m_lastActiveStatesCached.end(),
		[newState](const nlohmann::json& entry) {
			return entry.contains("nodeId") && entry["nodeId"] == newState["nodeId"];
		});

	// This state wasn't active last frame. Reinitialize it.
	// @todo Conditional reset on reinitialize. Property on the node in details.
	if (it == m_lastActiveStatesCached.end())
	{
		AFGraphNodeRegistry::Get().GetNode(newState["nodeId"])->OnBecomeRelevant();
	}

	m_lastActiveStates.push_back(newState);
}

std::string AFEvaluator::GetLastActiveStates()
{
	return m_lastActiveStates.dump();
}

void AFEvaluator::ClearLastActiveStates()
{
	m_lastActiveStatesCached = m_lastActiveStates;
	m_lastActiveStates = nlohmann::json::array();
}

void AFEvaluator::ClearPreEvaluationState()
{
	m_preevaluated.clear();
}

void AFEvaluator::ClearEvaluationState()
{
	m_evaluated.clear();
}

void AFEvaluator::AddSamplingState(const FAFStateSampling& sampling)
{
	auto it = std::find_if(m_samplingState.begin(), m_samplingState.end(), [sampling](const FAFStateSampling& cachedSampling)
		{
			return cachedSampling.nodeId == sampling.nodeId;
		});
	if (it == m_samplingState.end())
	{
		m_samplingState.push_back(sampling);
	}
}

std::vector<FAFStateSampling> AFEvaluator::GetCachedSamplingState(const std::string& context) const
{
	std::vector<FAFStateSampling> ret = {};

	for(const FAFStateSampling& sampling : m_samplingStateCached)
	{
		// Check all subcontexts, ie. From state could have its own states inside.
		if (IsEqualOrSubcontext(sampling.contextId, context))
		{
			ret.push_back(sampling);
		}
	}

	return ret;
}

bool AFEvaluator::IsEqualOrSubcontext(const std::string& question, const std::string& context) const
{
	if (question == context)
	{
		return true;
	}

	std::shared_ptr<AFGraphNode> main = AFGraphNodeRegistry::Get().GetNode(context);
	if (main)
	{
		for (const std::string& sub : main->GetSubNodes())
		{
			if (IsEqualOrSubcontext(question, sub))
			{
				return true;
			}
		}
	}

	return false;
}

void AFEvaluator::ClearSamplingState()
{
	m_samplingStateCached = m_samplingState;
	m_samplingState.clear();
}

bool AFEvaluator::GetAnimPaused() const
{
	return m_animPaused;
}

float AFEvaluator::GetAnimPlayrate() const
{
	return m_animPlayrate;
}

std::string AFEvaluator::GetPlayrateStatus() const
{
	nlohmann::json j = {
		{"Paused",   m_animPaused },
		{"Playrate", m_animPaused ? 0.0f : m_animPlayrate }
	};

	return j.dump();
}

void AFEvaluator::PlayrateSlower()
{
	m_animPlayrate = glm::clamp(m_animPlayrate - 0.1f, 0.0f, 2.0f);
}

void AFEvaluator::PlayrateToggle()
{
	m_animPaused = !m_animPaused;
}

void AFEvaluator::PlayrateFaster()
{
	m_animPlayrate = glm::clamp(m_animPlayrate + 0.1f, 0.0f, 2.0f);
}

EAFEvaluationMode AFEvaluator::GetEvaluationMode() const
{
	return m_evaluationMode;
}

void AFEvaluator::SetEvaluationMode(EAFEvaluationMode mode)
{
	m_evaluationMode = mode;
}

void AFEvaluator::UpdateSyncGroups(const std::string& nodeId, class IAFSyncGroupInterface* node)
{
	if (!node)
	{
		return;
	}

	if (node->GetSyncGroupName().empty())
	{
		return;
	}

	FAFSyncGroupProperties properties;
	properties.name = node->GetSyncGroupName();
	properties.mode = node->GetSyncGroupMode();

	m_syncGroups[nodeId] = properties;
}

bool AFEvaluator::GetSyncGroupProperties(const std::string& nodeId, FAFSyncGroupProperties& properties)
{
	auto it = m_syncGroups.find(nodeId);
	if (it == m_syncGroups.end())
	{
		return false;
	}

	properties = it->second;
	return true;
}

bool AFEvaluator::GetSyncGroupDriverTime(const std::string& syncGroup, float& driverTime)
{
	for (const auto& [nodeId, syncGroupProperties] : m_syncGroups)
	{
		if (syncGroupProperties.mode == EAFSyncGroupMode::Driver && syncGroupProperties.name == syncGroup)
		{
			std::shared_ptr<AFGraphNode> node = AFGraphNodeRegistry::Get().GetNode(nodeId);
			if (node)
			{
				IAFSyncGroupInterface* syncGroupInterface = dynamic_cast<IAFSyncGroupInterface*>(node.get());
				if (syncGroupInterface)
				{
					driverTime = syncGroupInterface->GetLocalTime();
					return true;
				}
			}
		}
	}

	return false;
}

bool AFEvaluator::GetSyncGroupDriverClip(const std::string& syncGroup, AFAnimationClip*& clip)
{
	for (const auto& [nodeId, syncGroupProperties] : m_syncGroups)
	{
		if (syncGroupProperties.mode == EAFSyncGroupMode::Driver && syncGroupProperties.name == syncGroup)
		{
			std::shared_ptr<AFGraphNode> node = AFGraphNodeRegistry::Get().GetNode(nodeId);
			if (node)
			{
				IAFSyncGroupInterface* syncGroupInterface = dynamic_cast<IAFSyncGroupInterface*>(node.get());
				if (syncGroupInterface)
				{
					clip = syncGroupInterface->GetAnimClip();
					return true;
				}
			}
		}
	}

	return false;
}
