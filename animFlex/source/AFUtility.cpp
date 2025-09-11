#include "AFUtility.h"

#include "AFAnimState.h"
#include "AFMath.h"
#include "AFCharacterMovementComponent.h"
#include "AFDebugShapeActor.h"
#include "AFGame.h"
#include "AFPlayerPawn.h"

float AFUtility::GetAngleTowardsMovementInput()
{
	std::shared_ptr<AFCharacterMovementComponent> m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();

	const glm::vec3& movementInput = glm::normalize(m_charMovement->GetLastPositiveMovementInput());
	const glm::quat& actorWorldRotation = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetRotationQuat();
	const glm::vec3& actorWorldForward = glm::normalize(actorWorldRotation * glm::vec3(0.0, 0.0f, 1.0f));

	const float angle = AFMath::SignedAngleBetweenVectors(movementInput, actorWorldForward, glm::vec3(0.0f, 1.0f, 0.0f));
	return angle;
}

float AFUtility::GetRootAngleTowardsMovementInput()
{
	std::shared_ptr<AFCharacterMovementComponent> m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	std::shared_ptr<AFAnimState> m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();

	if (!m_charMovement || !m_animState)
	{
		return 0.0f;
	}

	const glm::vec3& movementInput = glm::normalize(m_charMovement->GetLastPositiveMovementInput());
	const glm::quat& actorWorldRotation = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetRotationQuat();

	// We first get the actor rotation and then multiply the local offset to get root yaw in world space.
	const glm::quat& rootQuat = glm::quat(glm::angleAxis(glm::radians(m_animState->GetRootYaw()), glm::vec3(0.0f, 1.0f, 0.0f))) * actorWorldRotation;
	const glm::vec3& rootDir = glm::normalize(rootQuat * glm::vec3(0.0, 0.0f, 1.0f));

	const float angle = AFMath::SignedAngleBetweenVectors(movementInput, rootDir, glm::vec3(0.0f, 1.0f, 0.0f));

	return angle;
}

float AFUtility::GetRootAngleTowardsVelocity()
{
	std::shared_ptr<AFCharacterMovementComponent> m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	std::shared_ptr<AFAnimState> m_animState = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent()->GetAnimState();

	if (!m_charMovement || !m_animState)
	{
		return 0.0f;
	}

	const glm::vec3& movementInput = glm::normalize(m_charMovement->GetLastPositiveMovementInput());
	const glm::quat& actorWorldVelocity = glm::normalize(m_charMovement->GetVelocity());

	// We first get the actor rotation and then multiply the local offset to get root yaw in world space.
	const glm::quat& rootQuat = glm::quat(glm::angleAxis(glm::radians(m_animState->GetRootYaw()), glm::vec3(0.0f, 1.0f, 0.0f))) * actorWorldVelocity;
	const glm::vec3& rootDir = glm::normalize(rootQuat * glm::vec3(0.0, 0.0f, 1.0f));

	const float angle = AFMath::SignedAngleBetweenVectors(movementInput, rootDir, glm::vec3(0.0f, 1.0f, 0.0f));

	return angle;
}

float AFUtility::GetAngleTowardsVelocity()
{
	std::shared_ptr<AFCharacterMovementComponent> m_charMovement = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetCharacterMovementComponent();
	const glm::vec3& vel = m_charMovement->GetVelocity();

	if (glm::length(vel) < glm::epsilon<float>())
	{
		return 0.0f;
	}

	const glm::vec3& velocity = glm::normalize(vel);
	const glm::quat& actorWorldRotation = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetRotationQuat();
	const glm::vec3& actorWorldForward = glm::normalize(actorWorldRotation * glm::vec3(0.0, 0.0f, 1.0f));

	const float angle = AFMath::SignedAngleBetweenVectors(velocity, actorWorldForward, glm::vec3(0.0f, 1.0f, 0.0f));
	return angle;
}

glm::vec3 AFUtility::GetColorVector(EAFColor color)
{
	switch (color)
	{
		case EAFColor::Red: return glm::vec3(1.0f, 0.0f, 0.0f);
		case EAFColor::Green: return glm::vec3(0.0f, 1.0f, 0.0f);
		case EAFColor::Blue: return glm::vec3(0.0f, 0.0f, 1.0f);
		default: return glm::vec3(0.0f, 0.0f, 0.0f);
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}

void AFUtility::DrawDebugBox(const glm::vec3& location, float size, float lifetime, EAFColor color, const glm::quat& rotation)
{
	std::shared_ptr<AFDebugShapeActor> debugBox = AFGame::GetGame()->GetScene().CreateObject<AFDebugShapeActor>();
	debugBox->AssignDebugShape(EAFDebugShape::Box, color);

	DrawDebugActor(debugBox, location, lifetime, rotation, glm::vec3(size), color);
}

void AFUtility::GetBone(const AFPose& pose, const std::string& bone, EAFBoneSpace space, glm::vec3& location, glm::vec3& rotation)
{
	std::shared_ptr<AFSkeletalMeshComponent> mesh = AFGame::GetGame()->GetScene().GetPlayerPawn()->GetMeshComponent();
	if (!mesh)
	{
		return;
	}

	std::shared_ptr<AFJoint> bonePtr = pose.GetJoint(bone);
	if (!bonePtr)
	{
		return;
	}

	switch (space)
	{
		case EAFBoneSpace::Local:
		{
			location = bonePtr->GetLocation();
			rotation = AFMath::EulerFromQuaternion(bonePtr->GetRotation());
			break;
		}
		case EAFBoneSpace::Global:
		{
			location = bonePtr->GetGlobalLocation();
			rotation = AFMath::EulerFromQuaternion(bonePtr->GetGlobalRotation());
			break;
		}
		case EAFBoneSpace::World:
		{
			glm::mat4 trs = mesh->GetWorldTransform() * 
				AFMath::ComposeTransform(bonePtr->GetGlobalLocation(), bonePtr->GetGlobalRotation(), glm::vec3(1.0f));

			glm::vec3 scale;
			glm::quat rot;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(trs, scale, rot, location, skew, perspective);

			rotation = AFMath::EulerFromQuaternion(rot);
			break;
		}
	}
}

void AFUtility::CCDIK(AFPose& pose, 
	const std::vector<FAFIKBoneProperties>& ikbones,
	const glm::vec3 targetLocation,
	const glm::quat targetRotation,
	bool applyEffectorRotation,
	float threshold,
	size_t maxIterations)
{
	if (ikbones.size() < 3)
	{
		return;
	}

	const size_t effectorIdx = ikbones.size() - 1;
	std::shared_ptr<AFJoint> effector = pose.GetJoint(ikbones[effectorIdx].boneName);
	if (!effector)
	{
		return;
	}

	for (size_t iteration = 0; iteration < maxIterations; ++iteration)
	{
		glm::vec3 effectorLocation = effector->GetGlobalLocation();

		if (glm::length(targetLocation - effectorLocation) < threshold)
		{
			return;
		}

		for (size_t i = ikbones.size(); i-- > 0;)
		{
			// Effector does nothing.
			if (i == effectorIdx)
			{
				continue;
			}

			std::shared_ptr<AFJoint> markerJoint = pose.GetJoint(ikbones[i].boneName);
			if (!markerJoint)
			{
				return;
			}

			const glm::vec3 location = markerJoint->GetGlobalLocation();
			const glm::quat rotationParent = markerJoint->GetParentBone().lock()->GetGlobalRotation();

			effectorLocation = effector->GetGlobalLocation();

			// Directions.
			const glm::vec3 effectorDir = glm::normalize(effectorLocation - location);
			const glm::vec3 targetDir = glm::normalize(targetLocation - location);

			// What's the required rotation.
			const glm::quat rot = glm::rotation(effectorDir, targetDir);

			// Find delta in local space.
			const glm::quat localRot = glm::normalize(glm::conjugate(rotationParent) * rot * rotationParent);

			// Set new local rotation by adding the offset.
			markerJoint->SetRotation(glm::normalize(localRot * markerJoint->GetRotation()));

			// Apply limits.
			ApplyLimits(pose, ikbones[i]);

			// Recalculate local matrix.
			markerJoint->CalculateLocalTRSMatrix();
			const glm::mat4 parentMatrix = markerJoint->GetParentBone().expired() ?
				glm::mat4(1.0f) :
				markerJoint->GetParentBone().lock()->GetNodeMatrix();

			// Recalculate whole chain downwards.
			markerJoint->RecalculateBone(parentMatrix,
				pose.m_nodeToJoint,
				pose.m_jointMatrices,
				pose.m_inverseBindMatrices,
				pose.m_jointDualQuats);
		}
	}

	// Apply effector rotation.
	// It should always be given in global space.
	if (applyEffectorRotation)
	{
		const glm::quat effectorParentRot = effector->GetParentBone().lock()->GetGlobalRotation();
		effector->SetRotation(glm::normalize(glm::conjugate(effectorParentRot) * targetRotation));

		// Recalculate the effector.
		effector->RecalculateBone(effector->GetParentBone().lock()->GetNodeMatrix(),
			pose.m_nodeToJoint,
			pose.m_jointMatrices,
			pose.m_inverseBindMatrices,
			pose.m_jointDualQuats);
	}
}

void AFUtility::ApplyLimits(AFPose& pose, const FAFIKBoneProperties& boneProperties)
{
	if (boneProperties.boneName.empty())
	{
		return;
	}

	std::shared_ptr<AFJoint> joint = pose.GetJoint(boneProperties.boneName);
	if (!joint)
	{
		return;
	}

	switch (boneProperties.boneType)
	{
		case EAFJointType::Hinge:
		{
			const glm::quat localRotation = glm::normalize(joint->GetRotation());
			const glm::vec3 hingeAxis = glm::normalize(boneProperties.hingeProperties.hingeAxis);

			// Take out the vector part out of the local rotation (vector around which local rotation currently happens).
			const glm::vec3 real = glm::vec3(localRotation.x, localRotation.y, localRotation.z);

			// Project the local rotation onto hinge axis (how much are we rotating in the hinge axis).
			float proj = glm::dot(real, hingeAxis);

			// Construct the vector part of the twist quaternion.
			const glm::vec3 twistVec = hingeAxis * proj;

			// Construct the twist and swing quaternions.
			const glm::quat twist = glm::normalize(glm::quat(localRotation.w, twistVec.x, twistVec.y, twistVec.z));
			const glm::quat swing = glm::normalize(localRotation * glm::conjugate(twist));

			// Clamp twist angle.
			const float twistAngle = 2.0f * std::atan2(glm::length(glm::vec3(twist.x, twist.y, twist.z)), twist.w);
			const float sign = (proj >= 0.0f) ? 1.0f : -1.0f;
			const float twistDeg = glm::degrees(twistAngle) * sign;
			const float clampedDeg = glm::clamp(twistDeg, 
				boneProperties.hingeProperties.minAngle, 
				boneProperties.hingeProperties.maxAngle);
			//printf("%f\n", twistDeg);

			// Reconstruct clamped quaternion.
			const glm::quat clampedLocalRot = glm::angleAxis(glm::radians(clampedDeg), hingeAxis);
			joint->SetRotation(clampedLocalRot);

			break;
		}
		case EAFJointType::Ball:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void AFUtility::DrawDebugActor(std::shared_ptr<AFDebugShapeActor> actor, const glm::vec3& location, float lifetime,
                               const glm::quat& rotation, const glm::vec3& scale, EAFColor color)
{
	const_cast<AFScene&>(AFGame::GetGame()->GetScene()).AddActor(actor);

	actor->SetLocation(location);
	actor->SetRotation(rotation);
	actor->SetScale(scale);

	actor->SetLifetime(lifetime);
}
