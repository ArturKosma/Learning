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

	// @todo Expand frontend to be able to define arrays.
	std::vector<FAFIKBoneProperties> ikbones;
	ikbones.push_back({ m_rootBone.GetValue() });
	ikbones.push_back({ m_middleBone.GetValue() });
	ikbones.push_back({ m_effectorBone.GetValue() });

	AFUtility::CCDIK(ikpose,
		ikbones,
		m_targetLoc.GetValue(),
		glm::quat(glm::radians(m_targetRot.GetValue())));

	// Blend the alpha.
	m_currentAlpha = AFMath::FInterpToConst(m_currentAlpha,
		m_active ? 1.0f : 0.0f, 
		m_active ? blendInTime : blendOutTime,
		deltaTime);

	// Blend poses.
	AFMath::BlendPoses(m_blendedPose, pose, ikpose, m_currentAlpha);

	m_outputPose.SetValue(m_blendedPose);
}
