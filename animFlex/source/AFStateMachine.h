#pragma once

#include <stack>
#include "AFAnimGraph.h"
#include "AFGraphNode_StateStart.h"

class AFStateMachine : public AFAnimGraph
{
public:

	void PreEvaluate(float deltaTime) override;
	void Evaluate(float deltaTime) override;
	void Reset();

	void OnNodeCreated(const std::string& msg) override;
	void OnConnectionCreated(const std::string& msg);
	void OnConnectionRemoved(const std::string& msg);

	bool ScheduleBlend(const std::string& blendFrom, const std::string& blendTo, float blendLength);

	int m_maxTransitionsPerTick = 5;

private:

	std::weak_ptr<AFGraphNode_StateStart> m_stateStartNode;
	std::weak_ptr<AFGraphNode> m_currentState;

	std::vector<std::weak_ptr<AFGraphNode>> m_states = {};
	std::vector<FAFStateConnection> m_connections = {};

	std::deque<std::shared_ptr<FAFBlendStack_Blender>> m_blendStack = {};
};
