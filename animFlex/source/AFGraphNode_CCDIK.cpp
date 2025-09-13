#include "AFGraphNode_CCDIK.h"

#include "AFGame.h"
#include "AFMath.h"
#include "AFMesh.h"
#include "AFPlayerPawn.h"

void AFGraphNode_CCDIK::Init()
{
	m_mesh = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent();
}

void AFGraphNode_CCDIK::Evaluate(float deltaTime)
{
	AFPose& pose = const_cast<AFPose&>(m_inputPose.GetValue());
	std::shared_ptr<AFJoint> effectorJoint = pose.GetJoint(m_effectorBone.GetValue());
	if (!effectorJoint || !m_mesh)
	{
		m_outputPose.SetValue(pose);
		return;
	}

	// Cache mesh.
	std::shared_ptr<AFSkeletalMeshComponent> mesh = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent();
	if (!mesh)
	{
		m_outputPose.SetValue(pose);
		return;
	}

	// Make sure bones have recalculated matrices, since we will be using their globals.
	pose.RecalculateSkeleton();

	// Get a copy of the joint chain, that will get modified by IK.
	AFPose ikpose = pose; 

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

	// Find target location.
	glm::vec3 targetLoc = m_targetLoc.GetValue();
	if (m_useActivationPosition)
	{
		// Activation fetches current effector's location and caches it.
		// We cache world location, so that the limb actually stays in place when moving.
		if (!m_lastFrameActive && m_active.GetValue())
		{
			AFUtility::GetBone(pose, m_effectorBone.GetValue(), EAFBoneSpace::World, m_activationPosition, m_activationRotation);
		}

		// Convert world location to global (mesh) space.
		glm::quat rootQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		float rootYaw = 0.0f;
		AFMath::QuaternionTwist(mesh->GetMesh()->rootJoint->GetGlobalRotation(),
			glm::vec3(0.0f, 1.0f, 0.0f),
			rootQuat,
			rootYaw);
		const glm::mat4 rootRot = glm::mat4_cast(rootQuat);

		targetLoc = glm::vec3(glm::inverse(rootRot) * glm::inverse(m_mesh->GetWorldTransform()) * glm::vec4(m_activationPosition, 1.0f));
	}

	// Find target rotation.
	glm::quat targetRot = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	if (m_applyEffectorRotation.GetValue())
	{
		targetRot = effectorJoint->GetGlobalRotation();
	}
	if (m_applyTargetRotation.GetValue())
	{
		targetRot = glm::quat(glm::radians(m_targetRot.GetValue()));
	}

	AFUtility::CCDIK(ikpose,
		ikbones,
		targetLoc,
		targetRot);

	// Blend the alpha.
	m_currentAlpha = AFMath::FInterpToConst(m_currentAlpha,
		m_active ? 1.0f : 0.0f, 
		m_active ? blendInTime : blendOutTime,
		deltaTime);

	// Blend poses.
	AFMath::BlendPoses(m_blendedPose, pose, ikpose, m_currentAlpha);

	m_lastFrameActive = m_active.GetValue();
	m_outputPose.SetValue(m_blendedPose);
}
