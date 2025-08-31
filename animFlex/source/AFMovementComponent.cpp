#include "AFMovementComponent.h"
#include "AFMath.h"
#include "AFActor.h"
#include "AFTimerManager.h"

void AFMovementComponent::Tick(float deltaTime)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if(!owner)
	{
		return;
	}

	if (glm::length(m_lastMovementInput) > std::numeric_limits<float>::epsilon())
	{
		m_accelerating = true;

		const glm::vec3 desiredVelocity = AFMath::GetSafeNormal(m_lastMovementInput) * GetMaxSpeed();
		const glm::vec3 deltaVelocity = desiredVelocity - m_velocity;

		float maxDelta = GetAcceleration() * deltaTime;
		if (glm::length(deltaVelocity) > maxDelta)
		{
			m_velocity += AFMath::GetSafeNormal(deltaVelocity) * maxDelta;
		}
		else
		{
			m_velocity = desiredVelocity;
		}
	}
	else
	{
		m_accelerating = false;

		const float speed = glm::length(m_velocity);
		const float newSpeed = glm::max(speed - (GetDeceleration() * deltaTime), 0.0f);
		m_velocity = AFMath::GetSafeNormal(m_velocity) * newSpeed;
	}

	if (glm::length(m_velocity) > std::numeric_limits<float>::epsilon())
	{
		m_lastPositiveVelocity = m_velocity;
		const glm::vec3 velocityDelta = m_velocity * deltaTime;
		owner->AddOffsetLocation(velocityDelta);
		m_lastLocationOffset = velocityDelta;
	}

	// Consume last input.
	m_lastMovementInput = glm::vec3(0.0f, 0.0f, 0.0f);
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

glm::vec3 AFMovementComponent::GetVelocity() const
{
	return m_velocity;
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
		m_lastMovementInput += movementInput;
		m_lastMovementInput = glm::normalize(m_lastMovementInput);
		m_lastPositiveMovementInput = m_lastMovementInput;
	}
}

glm::vec3 AFMovementComponent::GetMovementInput() const
{
	return m_lastMovementInput;
}

glm::vec3 AFMovementComponent::GetLastPositiveMovementInput() const
{
	return m_lastPositiveMovementInput;
}

glm::vec3 AFMovementComponent::GetLastLocationOffset() const
{
	return m_lastLocationOffset;
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
}
