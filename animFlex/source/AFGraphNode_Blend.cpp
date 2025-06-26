#include "AFGraphNode_Blend.h"

#include "AFJoint.h"

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

	AFPose a = blend_inputPoseA.GetValue();
	AFPose b = blend_inputPoseB.GetValue();
	AFPose finalPose;
	finalPose.ApplyJoints(a.GetJoints());

	for (size_t i = 0; i < a.GetJoints().size(); ++i)
	{
		glm::vec3 blendLoc = glm::mix(a.GetJoints()[i]->GetLocation(), b.GetJoints()[i]->GetLocation(), alpha);
		glm::quat blendRot = glm::slerp(a.GetJoints()[i]->GetRotation(), b.GetJoints()[i]->GetRotation(), alpha);
		glm::vec3 blendScale = glm::mix(a.GetJoints()[i]->GetScale(), b.GetJoints()[i]->GetScale(), alpha);

		finalPose.GetJoints()[i]->SetLocation(blendLoc);
		finalPose.GetJoints()[i]->SetRotation(blendRot);
		finalPose.GetJoints()[i]->SetScale(blendScale);

		finalPose.GetJoints()[i]->CalculateLocalTRSMatrix();
	}

	blend_outputPose.SetValue(finalPose);
}
