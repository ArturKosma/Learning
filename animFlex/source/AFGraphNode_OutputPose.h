#pragma once
#include "AFGraphNode.h"
#include "AFPose.h"

class AFGraphNode_OutputPose : public AFGraphNodeCRTP<AFGraphNode_OutputPose>
{
	inline static FAFGraphNodeClassRegistrar<AFGraphNode_OutputPose> _registrar_AFGraphNode_OutputPose = FAFGraphNodeClassRegistrar<AFGraphNode_OutputPose>("OutputPose");
	std::string GetNodeType() const override { return "OutputPose"; }

public:

	FAFParam<AFPose> Pose = FAFParam<AFPose>("Pose");
	inline static FAFGraphNodeParamRegistrar<AFGraphNode_OutputPose, AFPose> _registrar_Pose = FAFGraphNodeParamRegistrar<AFGraphNode_OutputPose, AFPose>("OutputPose", "Pose", {}, &AFGraphNode_OutputPose::Pose, "Input");


	void Evaluate(float deltaTime) override;
};