#include "AFGraphNode_Blend.h"

#include "AFJoint.h"
#include "AFMath.h"

AFGraphNode_Blend::AFGraphNode_Blend()
{
}

void AFGraphNode_Blend::Evaluate(float deltaTime)
{
	float alpha = blend_alpha.GetValue();
	alpha = glm::clamp(alpha, 0.0f, 1.0f);

	constexpr float EPS = 1e-5f;
	if (alpha <= EPS)
	{
		blend_outputPose.SetValue(blend_inputPoseA.GetValue());
		return;
	}
	else if (alpha >= 1.0f - EPS)
	{
		blend_outputPose.SetValue(blend_inputPoseB.GetValue());
		return;
	}

	const AFPose& a = blend_inputPoseA.GetValue();
	const AFPose& b = blend_inputPoseB.GetValue();
	const AFPose& finalPose = blend_outputPose.GetValue();

	AFMath::BlendPoses(finalPose, a, b, alpha);
}
