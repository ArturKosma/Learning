#include "AFCharacterMovementComponent.h"
#include "AFActor.h"
#include "AFMath.h"

float AFCharacterMovementComponent::GetAcceleration() const
{
	return 1500.0f;
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

	std::shared_ptr<AFActor> ownerActor = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (ownerActor)
	{
		if (glm::length(m_lastPositiveVelocity) <= std::numeric_limits<float>::epsilon())
		{
			return;
		}

		const glm::vec3 lastDirection = AFMath::GetSafeNormal(m_lastPositiveVelocity);
		const glm::vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
		const glm::quat lastDirectionQuat = glm::rotation(forward, lastDirection);

		const float rotateSpeed = 250.0f;

		// Keep rotating the actor towards last velocity or last consumed input if there's no velocity.
		ownerActor->SetRotation(AFMath::QInterpTo(ownerActor->GetRotationQuat(), lastDirectionQuat, glm::radians(rotateSpeed), deltaTime));
	}
}
