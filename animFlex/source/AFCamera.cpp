#include "AFCamera.h"
#include "AFMath.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AFUtil.h"

glm::vec3 AFCamera::GetRotationEuler() const
{
	return glm::degrees(glm::eulerAngles(m_rotation));
}

void AFCamera::Tick(float deltaTime)
{
	if(!m_accelerating)
	{
		const float speed = glm::length(m_velocity);
		const float newSpeed = glm::max(speed - (m_deceleration * deltaTime), 0.0f);
		m_velocity = AFMath::GetSafeNormal(m_velocity) * newSpeed;
	}

	m_position = m_position + (m_velocity * deltaTime);
	m_accelerating = false;
}

void AFCamera::SetLocation(glm::vec3 newLocation)
{
	m_position = newLocation;
}

void AFCamera::AddOffsetLocation(glm::vec3 offset)
{
	m_position += offset;
}

void AFCamera::AddRotation(glm::vec3 eulerToAdd)
{
	if(glm::length(eulerToAdd) <= std::numeric_limits<float>::epsilon())
	{
		return;
	}

	m_controlPitch = glm::clamp(m_controlPitch + eulerToAdd.x, -89.0f, 89.0f);
	m_controlYaw += (eulerToAdd.y * -1.0f);

	const glm::quat quatPitch = glm::angleAxis(glm::radians(m_controlPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat quatYaw = glm::angleAxis(glm::radians(m_controlYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	m_rotation = glm::normalize(quatYaw * quatPitch);
}

void AFCamera::AddMovementInput(glm::vec3 movementInput)
{
	if(glm::length(movementInput) > std::numeric_limits<float>::epsilon())
	{
		m_accelerating = true;

		m_velocity = m_velocity + (AFMath::GetSafeNormal(movementInput) * m_acceleration * AFUtil::GetDeltaTime());

		if (glm::length(m_velocity) > m_maxSpeed)
		{
			m_velocity = glm::normalize(m_velocity) * m_maxSpeed;
		}
	}
}

glm::vec3 AFCamera::GetPosition() const
{
	return m_position;
}

glm::quat AFCamera::GetRotation() const
{
	return m_rotation;
}

glm::mat4 AFCamera::GetViewMatrix() const
{
	const glm::vec3 forward = glm::normalize(m_rotation * glm::vec3(0.0f, 0.0f, -1.0f));
	const glm::vec3 target = m_position + forward;

	return glm::lookAt(m_position, target, glm::vec3(0.0f, 1.0f, 0.0f));
}
