#pragma once
#include <memory>
#include <vector>
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

	void AddLastActiveSocket(const nlohmann::json& newSocket);
	std::string GetLastActiveSockets();
	void ClearLastActiveSockets();

	void AddLastActiveState(const nlohmann::json& newState);
	std::string GetLastActiveStates();
	void ClearLastActiveStates();

	void ClearEvaluationState();

private:

	nlohmann::json m_lastActiveSockets = nlohmann::json::array();
	nlohmann::json m_lastActiveStates = nlohmann::json::array();
	std::vector<std::shared_ptr<class AFGraphNode>> m_evaluated = {};
};
