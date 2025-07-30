#include "AFMovementComponent.h"
#include "AFMath.h"

#include <glm/ext/quaternion_trigonometric.hpp>

#include "AFActor.h"
#include "AFTimerManager.h"

void AFMovementComponent::Tick(float deltaTime)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if(!owner)
	{
		return;
	}

	if (!m_accelerating)
	{
		const float speed = glm::length(m_velocity);
		const float newSpeed = glm::max(speed - (GetDeceleration() * deltaTime), 0.0f);
		m_velocity = AFMath::GetSafeNormal(m_velocity) * newSpeed;
	}

	owner->AddOffsetLocation(m_velocity * deltaTime);
	m_accelerating = false;
}

float AFMovementComponent::GetAcceleration() const
{
	return m_acceleration;
}

float AFMovementComponent::GetDeceleration() const
{
	return m_deceleration;
}

float AFMovementComponent::GetMaxSpeed() const
{
	return m_maxSpeed;
}

void AFMovementComponent::SetControlRotation(const glm::vec3& newControlRotation)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	m_controlPitch = glm::clamp(newControlRotation.x, -89.0f, 89.0f);
	m_controlYaw = newControlRotation.y;

	glm::quat quatPitch = glm::angleAxis(glm::radians(m_controlPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat quatYaw = glm::angleAxis(glm::radians(m_controlYaw), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat quatRoll = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::quat rotation = quatRoll * quatYaw * quatPitch;

	owner->SetRotation(glm::normalize(rotation));
}

glm::vec3 AFMovementComponent::GetControlRotation() const
{
	return { glm::vec3(m_controlPitch, m_controlYaw, 0.0f) };
}

void AFMovementComponent::AddMovementInput(const glm::vec3& movementInput)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	if (glm::length(movementInput) > std::numeric_limits<float>::epsilon())
	{
		m_accelerating = true;

		m_velocity = m_velocity + (AFMath::GetSafeNormal(movementInput) * GetAcceleration() * AFTimerManager::GetDeltaTime());

		if (glm::length(m_velocity) > GetMaxSpeed())
		{
			m_velocity = glm::normalize(m_velocity) * GetMaxSpeed();
		}
	}
}

void AFMovementComponent::AddOffset(const glm::vec3& offset)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	owner->AddOffsetLocation(offset);
}

void AFMovementComponent::AddControlRotation(const glm::vec3& delta)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	if (glm::length(delta) <= std::numeric_limits<float>::epsilon())
	{
		return;
	}

	m_controlPitch = glm::clamp(m_controlPitch + delta.x, -89.0f, 89.0f);
	m_controlYaw += delta.y;
	m_controlYaw = m_controlYaw > 360.0f ? m_controlYaw - 360.0f : m_controlYaw;
	m_controlYaw = m_controlYaw < -360.0f ? m_controlYaw + 360.0f : m_controlYaw;

	const glm::quat quatPitch = glm::angleAxis(glm::radians(m_controlPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat quatYaw = glm::angleAxis(glm::radians(m_controlYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	owner->SetRotation(glm::normalize(quatYaw * quatPitch));
}
