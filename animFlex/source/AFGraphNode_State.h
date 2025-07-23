#pragma once
#include "AFGraphNode.h"

class AFGraphNode_State : public AFGraphNodeCRTP<AFGraphNode_State>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_State> _registrar_AFGraphNode_State = FAFGraphNodeClassRegistrar<AFGraphNode_State>("State");
	std::string GetNodeType() const override { return "State"; }

public:

	void Evaluate(float deltaTime) override;
};