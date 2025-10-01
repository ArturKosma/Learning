#pragma once
#include "AFGraphNode.h"
#include "AFAnimGraph.h"

class AFGraphNode_State : public AFGraphNodeCRTP<AFGraphNode_State>
{
	AFCLASS(AFGraphNode_State, "State", "NoDropdown")

public:

	AFPARAM(std::string, m_stateClassName, "", "State Class", "", "");
	AFPARAM(std::string, m_onEnterFunStr, "", "On Enter", "", "");
	AFPARAM(std::string, m_onTickFunStr, "", "On Tick", "", "");
	AFPARAM(bool, m_alwaysOnEnter, false, "Always Call OnEnter", "", "");

	void Init() override;
	void OnUpdate() override;
	void Evaluate(float deltaTime) override;
	void OnBecomeRelevant() override;
	std::shared_ptr<AFAnimGraph> GetGraph() const;
	std::shared_ptr<AFStateClass> GetStateObj() const;

protected:

	std::shared_ptr<AFAnimGraph> m_graph = nullptr;
	std::shared_ptr<AFStateClass> m_stateObj = nullptr;
};