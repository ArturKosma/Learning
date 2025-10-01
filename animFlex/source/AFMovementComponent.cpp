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

	const glm::vec3 accel = m_lastMovementInput * GetAcceleration();
	const bool hasAccel = glm::length2(accel) > eps;

	if (hasAccel)
	{
		m_velocity += accel * deltaTime;

		// Forwardness. Reduce speed when turning.
		float dot = glm::dot(AFMath::GetSafeNormal(m_velocity), AFMath::GetSafeNormal(accel));
		dot = glm::clamp(dot, 0.0f, 1.0f);
		dot = -glm::pow(dot, 4.0f) + 1.0f;

		float speed = glm::length(m_velocity);
		speed -= dot * GetLateralDeceleration() * deltaTime;
		speed = glm::clamp(speed, 0.0f, GetMaxSpeed());

		m_velocity = AFMath::GetSafeNormal(m_velocity) * speed;

		// Linear change of velocity, with speed maintained.
		//const glm::vec3 desiredVel = AFMath::GetSafeNormal(accel) * speed;
		glm::vec3 velDelta = accel - m_velocity;
		if (glm::length(velDelta) > GetMaxTurnRate())
		{
			velDelta = AFMath::GetSafeNormal(velDelta) * GetMaxTurnRate();
		}
		m_velocity += velDelta * deltaTime;
		m_velocity = AFMath::GetSafeNormal(m_velocity) * speed;
	}
	else
	{
		float speed = glm::length(m_velocity);
		speed -= GetDeceleration() * deltaTime;
		speed = glm::clamp(speed, 0.0f, GetMaxSpeed());
		m_velocity = AFMath::GetSafeNormal(m_velocity) * speed;
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

	// Input angle.
	// Project to XZ (ignore vertical).
	glm::vec3 curr = glm::vec3(m_lastMovementInput.x, 0.0f, m_lastMovementInput.z);
	const float len = glm::length(curr);
	if (len >= std::numeric_limits<float>::epsilon())
	{
		curr /= len;

		glm::vec3 prev = glm::vec3(m_lastFinalMovementInput.x, 0.0f, m_lastFinalMovementInput.z);
		float prevLen = glm::length(prev);
		if (prevLen <= std::numeric_limits<float>::epsilon())
		{
			// First valid sample, no delta.
			m_inputYawDelta = 0.0f;
		}
		else
		{
			prev /= prevLen;

			// Signed angle in degrees, range [-180, 180].
			float d = glm::clamp(glm::dot(prev, curr), -1.0f, 1.0f);
			float y = glm::cross(prev, curr).y;
			float newDelta = glm::degrees(std::atan2(y, d));

			if (std::abs(newDelta) < 0.001f)
			{
				newDelta = 0.0f;
			}

			m_inputYawDelta = newDelta;
		}
	}
	else
	{
		// No input, no delta.
		m_inputYawDelta = 0.0f;
	}

	/*if (glm::abs(m_inputYawDelta) > glm::epsilon<float>())
	{
		printf("%f\n", m_inputYawDelta);
	}*/
	//printf("last positive: %f, %f, %f\n", m_lastPositiveMovementInput.x, m_lastPositiveMovementInput.y, m_lastPositiveMovementInput.z);

	m_lastFinalMovementInput = m_lastMovementInput;
}

float AFMovementComponent::GetAcceleration() const
{
	return m_acceleration;
}

float AFMovementComponent::GetLateralDeceleration() const
{
	return 1500.0f;
}

float AFMovementComponent::GetMaxTurnRate() const
{
	return 2000.0f;
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

float AFMovementComponent::GetInputYawDelta() const
{
	return m_inputYawDelta;
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
