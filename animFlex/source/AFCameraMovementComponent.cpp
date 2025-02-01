#include "AFCameraMovementComponent.h"

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
	m_cameraSpeedMultiplier += (direction / 3.5f);

	m_cameraSpeedMultiplier = glm::clamp(m_cameraSpeedMultiplier, 0.1f, 10.0f);
}
