#include "AFGraphNode_Blend.h"

AFGraphNode_Blend::AFGraphNode_Blend()
{
}

void AFGraphNode_Blend::Evaluate(float deltaTime)
{
	const AFPose& a = inputPoseA.GetValue();

	outputPose.SetValue(a);
}
