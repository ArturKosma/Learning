#pragma once
#include "AFAnimGraph.h"
#include "AFGraphNode_State.h"
#include "AFGraphNode_StateStart.h"

class AFStateMachine : public AFAnimGraph
{
public:

	void Evaluate(float deltaTime) override;

	void OnNodeCreated(const std::string& msg) override;
	void OnConnectionCreated(const std::string& msg);
	void OnConnectionRemoved(const std::string& msg);

	void ScheduleBlend(const std::string& blendFrom, const std::string& blendTo, float blendLength);
	void ClearBlend();

private:

	std::weak_ptr<AFGraphNode_StateStart> m_stateStartNode;
	std::weak_ptr<AFGraphNode> m_currentState;

	std::vector<std::weak_ptr<AFGraphNode>> m_states = {};
	std::vector<FAFStateConnection> m_connections = {};

	bool m_isBlending = false;
	FAFStateBlend m_currentBlend = {};
};
