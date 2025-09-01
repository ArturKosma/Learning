#pragma once
#include <memory>
#include <vector>

#include "AFStructs.h"
#include "third_party/json.hpp"

class AFEvaluator
{
public:
	static AFEvaluator& Get()
	{
		static AFEvaluator evaluator;
		return evaluator;
	}

	void EvaluateNode(std::shared_ptr<class AFGraphNode> node);
	void PreEvaluateNode(std::shared_ptr<class AFGraphNode> node);

	void AddLastActiveSocket(const nlohmann::json& newSocket);
	std::string GetLastActiveSockets();
	void ClearLastActiveSockets();

	void AddLastActiveState(const nlohmann::json& newState);
	std::string GetLastActiveStates();
	void ClearLastActiveStates();

	void ClearPreEvaluationState();
	void ClearEvaluationState();

	void AddSamplingState(const FAFStateSampling& sampling);
	std::vector<FAFStateSampling> GetCachedSamplingState(const std::string& context) const;
	void ClearSamplingState();

	bool GetAnimPaused() const;
	float GetAnimPlayrate() const;
	std::string GetPlayrateStatus() const;
	void PlayrateSlower();
	void PlayrateToggle();
	void PlayrateFaster();

private:

	nlohmann::json m_lastActiveSockets = nlohmann::json::array();
	nlohmann::json m_lastActiveStates = nlohmann::json::array();
	// Required to compare if new active state is actually new or was active a frame before.
	nlohmann::json m_lastActiveStatesCached = nlohmann::json::array();

	std::vector<std::shared_ptr<class AFGraphNode>> m_evaluated = {};
	std::vector<std::shared_ptr<class AFGraphNode>> m_preEvaluated = {};

	std::vector<FAFStateSampling> m_samplingState = {};
	// Required to be able to look in the previous frame.
	std::vector<FAFStateSampling> m_samplingStateCached = {};

	bool m_animPaused = false;
	float m_animPlayrate = 1.0f;
};
