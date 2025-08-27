#include "AFCharacterMovementComponent.h"
#include "AFActor.h"
#include "AFAnimState.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

float AFCharacterMovementComponent::GetAcceleration() const
{
	return 1000.0f;
}

float AFCharacterMovementComponent::GetDeceleration() const
{
	return 1500.0f;
}

float AFCharacterMovementComponent::GetMaxSpeed() const
{
	return 500.0f;
}

void AFCharacterMovementComponent::Tick(float deltaTime)
{
	AFMovementComponent::Tick(deltaTime);

	std::shared_ptr<AFPlayerPawn> ownerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());
	if (ownerPawn)
	{
		if (glm::length(m_velocity) <= std::numeric_limits<float>::epsilon())
		{
			return;
		}

		std::shared_ptr<AFAnimState> animState = ownerPawn->GetMeshComponent()->GetAnimState();
		if (!animState)
		{
			return;
		}

		const glm::vec3 lastDirection = AFMath::GetSafeNormal(m_velocity);
		const glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		const glm::quat lastDirectionQuat = glm::rotation(forward, lastDirection);

		// Rotation from rootYawDelta, which acts like rootMotion for rotation.
		const float rootYawDelta = animState->GetCurveValue("rootYawDelta");

		// Scales how much we use mechanical rotation towards last velocity.
		const float rootYawAuthority = animState->GetCurveValue("rootYawAuthority");

		float rotateSpeedTowardsVel = 250.0f * (1.0f - rootYawAuthority);

		if (glm::abs(rootYawDelta) > glm::epsilon<float>())
		{
			ownerPawn->AddOffsetRotation(glm::vec3(0.0f, rootYawDelta, 0.0f));
		}
		else if (rotateSpeedTowardsVel > glm::epsilon<float>())
		{
			// Keep rotating the actor towards last velocity.
			ownerPawn->SetRotation(AFMath::QInterpTo(ownerPawn->GetRotationQuat(), lastDirectionQuat, glm::radians(rotateSpeedTowardsVel), deltaTime));
		}
	}
}
