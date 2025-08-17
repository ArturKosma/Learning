#pragma once
#include "AFGraphNode.h"
#include "AFAnimGraph.h"

class AFGraphNode_State : public AFGraphNodeCRTP<AFGraphNode_State>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_State> _registrar_AFGraphNode_State = FAFGraphNodeClassRegistrar<AFGraphNode_State>("State");
	std::string GetNodeType() const override { return "State"; }

public:

	void Init() override;
	void Evaluate(float deltaTime) override;
	void OnReset() override;
	std::shared_ptr<AFAnimGraph> GetGraph() const;

protected:

	std::shared_ptr<AFAnimGraph> m_graph = nullptr;
};