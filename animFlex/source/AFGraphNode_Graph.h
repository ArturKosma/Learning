#pragma once
#include "AFGraphNode.h"
#include "AFAnimGraph.h"

class AFGraphNode_Graph : public AFGraphNodeCRTP<AFGraphNode_Graph>
{
	AFCLASS(AFGraphNode_Graph, "Graph", "")

public:

	AFPARAM(AFPose, GraphOutput, {}, "", "Output", "");

	void Init() override;

	void Evaluate(float deltaTime) override;
	std::shared_ptr<AFAnimGraph> GetGraph() const;

protected:

	std::shared_ptr<AFAnimGraph> m_graph = nullptr;
};