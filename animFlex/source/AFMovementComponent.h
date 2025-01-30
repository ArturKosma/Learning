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

	void AddControlRotation(const glm::vec3& eulerToAdd);
	void AddMovementInput(const glm::vec3& movementInput);

private:

	glm::vec3 m_velocity = glm::vec3(0.0f);
	float m_deceleration = 10.0f;
	float m_acceleration = 50.0f;
	float m_maxSpeed = 2.0f;

	float m_controlPitch = 0.0f;
	float m_controlYaw = 0.0f;

	bool m_accelerating = false;
};
