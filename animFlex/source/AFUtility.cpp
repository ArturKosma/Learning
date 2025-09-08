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

void AFUtility::DrawDebugActor(std::shared_ptr<AFDebugShapeActor> actor, const glm::vec3& location, float lifetime,
                               const glm::quat& rotation, const glm::vec3& scale, EAFColor color)
{
	const_cast<AFScene&>(AFGame::GetGame()->GetScene()).AddActor(actor);

	actor->SetLocation(location);
	actor->SetRotation(rotation);
	actor->SetScale(scale);

	actor->SetLifetime(lifetime);
}
