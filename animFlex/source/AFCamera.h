#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AFStructs.h"

class AFCamera
{
public:

	void Tick(float deltaTime);

	void SetLocation(glm::vec3 newLocation);
	void AddOffsetLocation(glm::vec3 offset);
	void AddRotation(glm::vec3 eulerToAdd);
	void AddMovementInput(glm::vec3 movementInput);

	glm::vec3 GetPosition() const;
	glm::quat GetRotation() const;
	glm::vec3 GetRotationEuler() const;

	glm::mat4 GetViewMatrix() const;

private:

	glm::vec3 m_position = glm::vec3(0.4f, 0.3f, 1.0f);
	glm::quat m_rotation = glm::quat(0.0f, 0.0f, 0.0f, 1.0f);

	glm::vec3 m_velocity = glm::vec3(0.0f);
	float m_deceleration = 10.0f;
	float m_acceleration = 50.0f;
	float m_maxSpeed = 2.0f;

	float m_controlPitch = 0.0f;
	float m_controlYaw = 0.0f;

	bool m_accelerating = false;
};
