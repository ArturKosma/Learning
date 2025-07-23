#pragma once
#include "AFAnimGraph.h"
#include "AFGraphNode_StateStart.h"

class AFStateMachine : public AFAnimGraph
{
public:

	void OnNodeCreated(const std::string& msg) override;

private:

	std::shared_ptr<AFGraphNode_StateStart> m_stateStartNode = nullptr;
};
