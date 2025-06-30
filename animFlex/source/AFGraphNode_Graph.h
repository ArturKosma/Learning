#pragma once
#include "AFGraphNode.h"

class AFGraphNode_Graph : public AFGraphNodeCRTP<AFGraphNode_Graph>
{
	AFCLASS(AFGraphNode_Graph, "Graph", "")

public:

	AFPARAM(AFPose, GraphOutput, "", "Output", "");

	void Evaluate(float deltaTime) override;
};