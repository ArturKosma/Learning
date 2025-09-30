#include "AFMovementComponent.h"
#include "AFMath.h"
#include "AFActor.h"
#include "AFTimerManager.h"

void AFMovementComponent::PostTick(float deltaTime)
{
	m_lastMovementInput = glm::vec3(0.0f, 0.0f, 0.0f);
	m_lastLocalMovementInput = glm::vec3(0.0f, 0.0f, 0.0f);
}

void AFMovementComponent::PreTick(float deltaTime)
{
	
}

void AFMovementComponent::Tick(float deltaTime)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if(!owner)
	{
		return;
	}

	const float eps = 1e-4f;
	const float maxSpeed = GetMaxSpeed();
	const float maxAccel = GetAcceleration();
	const float maxDecel = GetDeceleration();

	const glm::vec3 accel = m_lastMovementInput * maxAccel;
	const bool hasAccel = glm::length2(accel) > eps;

	// Either no input or reverse direction.
	const bool shouldBrake = !hasAccel || glm::dot(accel, m_velocity) <= 0.0f;

	if (shouldBrake && (glm::length2(m_velocity) > eps))
	{
		m_accelerating = false;

		const glm::vec3 dir = m_speed < eps ? AFMath::GetSafeNormal(m_lastMovementInput) : m_velocity / m_speed;

		// Decelerate.
		m_speed -= maxDecel * deltaTime;
		m_speed = glm::clamp(m_speed, 0.0f, maxSpeed);

		m_velocity = dir * m_speed;
	}
	else if (hasAccel)
	{
		m_accelerating = true;

		const glm::vec3 dir = m_speed < eps ? AFMath::GetSafeNormal(m_lastMovementInput) : m_velocity / m_speed;

		// Accelerate.
		m_speed += maxAccel * deltaTime;
		m_speed = glm::clamp(m_speed, 0.0f, maxSpeed);

		m_velocity = dir * m_speed;
	}

	// "Turn" velocity.
	if (hasAccel)
	{
		const glm::vec3 desiredVelocity = AFMath::GetSafeNormal(m_lastMovementInput) * m_speed;

		// Lower the speed during turning.
		const float desiredDot = glm::dot(AFMath::GetSafeNormal(desiredVelocity), AFMath::GetSafeNormal(m_velocity));
		const float desiredTurnSpeedModifier = AFMath::MapRangeClamped(desiredDot, 0.0f, 0.9f, 0.1f, 1.0f);
		m_turnSpeedModifier = AFMath::FInterpToConst(m_turnSpeedModifier, desiredTurnSpeedModifier, 3.0f, deltaTime);

		glm::vec3 delta = desiredVelocity - m_velocity;
		const float maxTurnDelta = GetMaxVelocityTurnSpeed() * deltaTime;
		const float dlen = glm::length(delta);
		if (dlen > maxTurnDelta && dlen > eps)
		{
			delta *= (maxTurnDelta / dlen);
		}

		const glm::vec3 newVel = m_velocity + delta;
		m_velocity = AFMath::GetSafeNormal(newVel) * (m_speed * m_turnSpeedModifier);
		printf("%f\n", m_speed * m_turnSpeedModifier);
	}

	if (glm::length(m_velocity) > eps)
	{
		m_lastPositiveVelocity = m_velocity;
		const glm::vec3 velocityDelta = m_velocity * deltaTime;

		// Offset location.
		owner->AddOffsetLocation(velocityDelta);
		m_lastLocationOffset = velocityDelta;
	}
	else
	{
		m_lastLocationOffset = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	// Cache deltas.
	m_lastControlYawDelta = GetControlRotation().y - m_lastFrameControlRotation.y;
	m_lastFrameControlRotation = GetControlRotation();
}

float AFMovementComponent::GetAcceleration() const
{
	return m_acceleration;
}

float AFMovementComponent::GetMaxVelocityTurnSpeed() const
{
	return 1000.0f;
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
	m_controlYaw = m_controlYaw > 360.0f ? m_controlYaw - 360.0f : m_controlYaw;
	m_controlYaw = m_controlYaw < -360.0f ? m_controlYaw + 360.0f : m_controlYaw;
}

glm::vec3 AFMovementComponent::GetControlRotation() const
{
	return { glm::vec3(m_controlPitch, m_controlYaw, 0.0f) };
}

void AFMovementComponent::AddMovementInput(const glm::vec3& movementInput, const glm::vec3& localMovementInput)
{
	std::shared_ptr<AFActor> owner = std::dynamic_pointer_cast<AFActor>(GetOwner().lock());
	if (!owner)
	{
		return;
	}

	if (glm::length(movementInput) > std::numeric_limits<float>::epsilon())
	{
		m_lastMovementInput += movementInput;

		// #hack.
		// This won't work with gamepad and scaled stroke.
		// @todo Gamepad support.
		m_lastMovementInput = glm::normalize(m_lastMovementInput);
		m_lastPositiveMovementInput = m_lastMovementInput;
	}

	// Cache local movement input,
	// to be able to tell if user changed key-direction without rotating camera.
	m_lastLocalMovementInput += localMovementInput;
}

glm::vec3 AFMovementComponent::GetMovementInput() const
{
	return m_lastMovementInput;
}

glm::vec3 AFMovementComponent::GetLastPositiveMovementInput() const
{
	return m_lastPositiveMovementInput;
}

glm::vec3 AFMovementComponent::GetLastLocalMovementInput()
{
	return m_lastLocalMovementInput;
}

glm::vec3 AFMovementComponent::GetLastFrameControlRotation() const
{
	return m_lastFrameControlRotation;
}

glm::vec3 AFMovementComponent::GetLastLocationOffset() const
{
	return m_lastLocationOffset;
}

float AFMovementComponent::GetLastControlYawDelta()
{
	return m_lastControlYawDelta;
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

	m_controlPitch = glm::clamp(m_controlPitch + delta.x, -89.0f, 89.0f);
	m_controlYaw += delta.y;
	m_controlYaw = m_controlYaw > 360.0f ? m_controlYaw - 360.0f : m_controlYaw;
	m_controlYaw = m_controlYaw < -360.0f ? m_controlYaw + 360.0f : m_controlYaw;
}
