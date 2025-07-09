#pragma once
#include "AFExec.h"
#include "AFGraphNode.h"

class AFGraphNode_StateStart: public AFGraphNodeCRTP<AFGraphNode_StateStart>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_StateStart> _registrar_AFGraphNode_StateStart = FAFGraphNodeClassRegistrar<AFGraphNode_StateStart>("StateStart");
	std::string GetNodeType() const override { return "StateStart"; }

public:

	FAFParam<AFExec> Exec = FAFParam<AFExec>("Exec");
	inline static FAFGraphNodeParamRegistrar<AFGraphNode_StateStart, AFExec> _registrar_Exec = FAFGraphNodeParamRegistrar<AFGraphNode_StateStart, AFExec>("StateStart", "Exec", &AFGraphNode_StateStart::Exec, "Output");


	void Evaluate(float deltaTime) override;
};