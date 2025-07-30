#include "AFCameraMovementComponent.h"

#include <glm/common.hpp>

#include "AFActor.h"
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

void AFCameraMovementComponent::ResetControlRotation()
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	glm::quat q = owner->GetRotationQuat();

	// Extract pitch from quat.
	float sinPitch = -2.0f * (q.y * q.z - q.w * q.x);
	float pitch;

	if (std::abs(sinPitch) >= 1.0f)
		pitch = std::copysign(glm::half_pi<float>(), sinPitch); // Use 90 degrees if out of range.
	else
		pitch = std::asin(sinPitch);

	// Extract yaw from quat.
	float yaw = std::atan2(2.0f * (q.x * q.z + q.w * q.y), q.w * q.w - q.x * q.x - q.y * q.y + q.z * q.z);

	m_controlPitch = glm::clamp(glm::degrees(pitch), -89.0f, 89.0f);
	m_controlYaw = glm::degrees(yaw);

	// Wrap yaw.
	if (m_controlYaw > 360.0f) m_controlYaw -= 360.0f;
	if (m_controlYaw < -360.0f) m_controlYaw += 360.0f;
}
