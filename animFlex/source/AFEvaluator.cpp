#include "AFEvaluator.h"
#include "AFGraphNode.h"
#include "AFTimerManager.h"

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
		AFGraphNodeRegistry::Get().GetNode(newState["nodeId"])->OnReset();
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
		if (sampling.contextId == context)
		{
			ret.push_back(sampling);
		}
	}

	return ret;
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
