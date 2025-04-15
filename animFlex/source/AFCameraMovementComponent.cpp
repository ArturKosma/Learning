#include "AFCameraMovementComponent.h"

#include <glm/common.hpp>

#include "AFUtility.h"

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
	m_cameraSpeedMultiplier += (direction * 75.0f);

	m_cameraSpeedMultiplier = glm::clamp(m_cameraSpeedMultiplier, 150.0f, 1500.0f);
}
