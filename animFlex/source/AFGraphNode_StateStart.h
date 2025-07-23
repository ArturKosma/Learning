#pragma once
#include "AFGraphNode.h"

class AFGraphNode_StateStart: public AFGraphNodeCRTP<AFGraphNode_StateStart>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_StateStart> _registrar_AFGraphNode_StateStart = FAFGraphNodeClassRegistrar<AFGraphNode_StateStart>("StateStart");
	std::string GetNodeType() const override { return "StateStart"; }

public:

	void Evaluate(float deltaTime) override;
};