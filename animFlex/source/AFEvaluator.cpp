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
		node->Evaluate(AFTimerManager::GetDeltaTime());
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

void AFEvaluator::ClearEvaluationState()
{
	m_evaluated.clear();
}
