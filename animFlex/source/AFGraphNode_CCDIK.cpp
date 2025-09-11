#include "AFGraphNode_CCDIK.h"

#include "AFGame.h"
#include "AFMath.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"

void AFGraphNode_CCDIK::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());
	AFPose ikpose = pose; // Get a copy of the joint chain, that will get modified by IK.

	// Safe division.
	const float blendInTime = AFMath::NearlyEqual(m_blendInTime.GetValue(), 0.0f) ? 0.0f : 1.0f / m_blendInTime.GetValue();
	const float blendOutTime = AFMath::NearlyEqual(m_blendOutTime.GetValue(), 0.0f) ? 0.0f : 1.0f / m_blendOutTime.GetValue();

	// @todo Expand frontend to be able to define this.
	// Currently this is hard coded to work for legs.
	std::vector<FAFIKBoneProperties> ikbones;

	// Limits.
	FAFHingeProperties hingeLimitProperties = { glm::vec3(0.0f, 0.0f, 1.0), -130.0f, -5.0f };
	FAFBallProperties ballLimitProperties = {};

	// Each bone properties.
	FAFIKBoneProperties rootProperties = { m_rootBone.GetValue(), EAFJointType::Ball, {}, ballLimitProperties };
	FAFIKBoneProperties middleProperties = { m_middleBone.GetValue(), EAFJointType::Hinge, hingeLimitProperties, {} };
	FAFIKBoneProperties effectorProperties = { m_effectorBone.GetValue(), EAFJointType::Ball, {}, ballLimitProperties };

	// Add them.
	ikbones.push_back(rootProperties);
	ikbones.push_back(middleProperties);
	ikbones.push_back(effectorProperties);

	AFUtility::CCDIK(ikpose,
		ikbones,
		m_targetLoc.GetValue(),
		glm::quat(glm::radians(m_targetRot.GetValue())),
		m_applyEffectorRotation);

	// Blend the alpha.
	m_currentAlpha = AFMath::FInterpToConst(m_currentAlpha,
		m_active ? 1.0f : 0.0f, 
		m_active ? blendInTime : blendOutTime,
		deltaTime);

	// Blend poses.
	AFMath::BlendPoses(m_blendedPose, pose, ikpose, m_currentAlpha);

	m_outputPose.SetValue(m_blendedPose);
}
