#pragma once
#include <glm/vec3.hpp>

#include "AFComponent.h"

class AFMovementComponent : public AFComponent
{
public:

	void Tick(float deltaTime);

	virtual float GetAcceleration() const;
	virtual float GetDeceleration() const;
	virtual float GetMaxSpeed() const;

	glm::vec3 GetVelocity() const;

	virtual void SetControlRotation(const glm::vec3& newControlRotation);
	glm::vec3 GetControlRotation() const;

	virtual void AddControlRotation(const glm::vec3& delta);
	virtual void AddMovementInput(const glm::vec3& movementInput);
	virtual void AddOffset(const glm::vec3& offset);

protected:

	glm::vec3 m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_lastPositiveVelocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_lastMovementInput = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_lastPositiveMovementInput = glm::vec3(0.0f, 0.0f, 0.0f);

	float m_deceleration = 10.0f;
	float m_acceleration = 50.0f;
	float m_maxSpeed = 2.0f;

	float m_controlPitch = 0.0f;
	float m_controlYaw = 0.0f;

	bool m_accelerating = false;
};
