#pragma once
#include "AFAnimGraph.h"
#include "AFGraphNode_OutputCond.h"

class AFAnimGraphCond : public AFAnimGraph
{
public:

	virtual void Evaluate(float deltaTime);
	virtual void OnNodeCreated(const std::string& msg);

	bool GetEvalResult() const;

private:

	std::shared_ptr<AFGraphNode_OutputCond> m_outputCondNode = nullptr;
	bool m_finalCond = false;
};

