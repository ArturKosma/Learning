#include "AFUtility.h"

#include "AFAnimState.h"
#include "AFMath.h"
#include "AFCharacterMovementComponent.h"
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
