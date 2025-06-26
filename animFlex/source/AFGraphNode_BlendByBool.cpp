#include "AFGraphNode_BlendByBool.h"

void AFGraphNode_BlendByBool::Evaluate(float deltaTime)
{
	if (blendBool_useA.GetValue())
	{
		blendBool_outputPose.SetValue(blendBool_inputPoseA.GetValue());
	}
	else
	{
		blendBool_outputPose.SetValue(blendBool_inputPoseB.GetValue());
	}
}
