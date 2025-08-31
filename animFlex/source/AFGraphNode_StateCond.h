#pragma once
#include "AFGraphNode.h"
#include "AFAnimGraphCond.h"

class AFGraphNode_StateCond : public AFGraphNodeCRTP<AFGraphNode_StateCond>
{
	AFCLASS(AFGraphNode_StateCond, "StateCond", "NoDropdown")

public:

	AFPARAM(float, m_blendTime, 0.15f, "Blend Time", "", "");

	void Init() override;
	void Evaluate(float deltaTime) override;
	std::shared_ptr<AFAnimGraphCond> GetGraph() const;

	bool GetLastEvaluation() const;

	void SetConnectionPoints(const std::string& from, const std::string& to);
	void GetConnectionPoints(std::string& from, std::string& to);

protected:

	std::shared_ptr<AFAnimGraphCond> m_graph = nullptr;
	std::string m_from = "";
	std::string m_to = "";

private:

	bool m_lastEval = false;
};