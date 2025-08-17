#pragma once
#include "AFGraphNode.h"
#include "AFAnimGraphCond.h"

class AFGraphNode_StateCond : public AFGraphNodeCRTP<AFGraphNode_StateCond>
{
public:

	inline static FAFGraphNodeClassRegistrar<AFGraphNode_StateCond> _registrar_AFGraphNode_StateCond = FAFGraphNodeClassRegistrar<AFGraphNode_StateCond>("StateCond");
	std::string GetNodeType() const override { return "StateCond"; }

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