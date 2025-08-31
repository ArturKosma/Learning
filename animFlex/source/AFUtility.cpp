#include "AFUtility.h"

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
