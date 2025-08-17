#include "AFGraphNode_StateCond.h"

void AFGraphNode_StateCond::Init()
{
	m_graph = std::make_shared<AFAnimGraphCond>();
}

void AFGraphNode_StateCond::Evaluate(float deltaTime)
{
	if (m_graph)
	{
		m_graph->Evaluate(deltaTime);
		m_lastEval = m_graph->GetEvalResult();
	}
}

std::shared_ptr<AFAnimGraphCond> AFGraphNode_StateCond::GetGraph() const
{
	return m_graph;
}

bool AFGraphNode_StateCond::GetLastEvaluation() const
{
	return m_lastEval;
}

void AFGraphNode_StateCond::SetConnectionPoints(const std::string& from, const std::string& to)
{
	m_from = from;
	m_to = to;
}

void AFGraphNode_StateCond::GetConnectionPoints(std::string& from, std::string& to)
{
	from = m_from;
	to = m_to;
}
