#include "AFMovementComponent.h"
#include "AFMath.h"

#include <glm/ext/quaternion_trigonometric.hpp>

#include "AFActor.h"
#include "AFTimerManager.h"

void AFMovementComponent::Tick(float deltaTime)
{
	AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
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

glm::vec3 AFMovementComponent::GetControlRotation() const
{
	return { glm::vec3(m_controlPitch, m_controlYaw, 0.0f) };
}

void AFMovementComponent::AddMovementInput(const glm::vec3& movementInput)
{
	AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
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
	AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
	if (!owner)
	{
		return;
	}

	owner->AddOffsetLocation(offset);
}

void AFMovementComponent::AddControlRotation(const glm::vec3& eulerToAdd)
{
	AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
	if (!owner)
	{
		return;
	}

	if (glm::length(eulerToAdd) <= std::numeric_limits<float>::epsilon())
	{
		return;
	}

	m_controlPitch = glm::clamp(m_controlPitch + eulerToAdd.x, -89.0f, 89.0f);
	m_controlYaw += eulerToAdd.y;

	const glm::quat quatPitch = glm::angleAxis(glm::radians(m_controlPitch), glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::quat quatYaw = glm::angleAxis(glm::radians(m_controlYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	owner->SetRotation(glm::normalize(quatYaw * quatPitch));
}
