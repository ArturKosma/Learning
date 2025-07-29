#include "AFCameraMovementComponent.h"

#include <glm/common.hpp>

#include "AFMath.h"

void AFCameraMovementComponent::AddControlRotation(const glm::vec3& eulerToAdd)
{
	AFMovementComponent::AddControlRotation(eulerToAdd);
}

void AFCameraMovementComponent::AddMovementInput(const glm::vec3& movementInput)
{
	AFMovementComponent::AddMovementInput(movementInput);
}

void AFCameraMovementComponent::AddOffset(const glm::vec3& offset)
{
	AFMovementComponent::AddOffset(offset);
}

float AFCameraMovementComponent::GetAcceleration() const
{
	return AFMovementComponent::GetAcceleration() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetDeceleration() const
{
	return AFMovementComponent::GetDeceleration() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetMaxSpeed() const
{
	return AFMovementComponent::GetMaxSpeed() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetCameraSpeedMultiplier() const
{
	return m_cameraSpeedMultiplier;
}

void AFCameraMovementComponent::AddCameraSpeedMultiplier(float direction)
{
	m_cameraSpeedMultiplier += (direction * 25.0f);

	m_cameraSpeedMultiplier = glm::clamp(m_cameraSpeedMultiplier, 25.0f, 600.0f);
}
