#pragma once
#include "AFGraphNode.h"

class AFGraphNode_OutputCond : public AFGraphNodeCRTP<AFGraphNode_OutputCond>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_OutputCond> _registrar_AFGraphNode_OutputCond = FAFGraphNodeClassRegistrar<AFGraphNode_OutputCond>("OutputCond");
	std::string GetNodeType() const override { return "OutputCond"; }

public:

	FAFParam<bool> OutputCond = FAFParam<bool>("OutputCond");
	inline static FAFGraphNodeParamRegistrar<AFGraphNode_OutputCond, bool> _registrar_OutputCond = FAFGraphNodeParamRegistrar<AFGraphNode_OutputCond, bool>("OutputCond", "OutputCond", &AFGraphNode_OutputCond::OutputCond, "Input");

	void Evaluate(float deltaTime) override;
};