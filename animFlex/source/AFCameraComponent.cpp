#include "AFCameraComponent.h"

glm::mat4 AFCameraComponent::GetViewMatrix() const
{
	const glm::vec3 forward = glm::normalize(GetWorldRotation() * glm::vec3(0.0f, 0.0f, -1.0f));
	const glm::vec3 target = GetWorldLocation() + forward;

	return glm::lookAt(GetWorldLocation(), target, glm::vec3(0.0f, 1.0f, 0.0f));
}

const AFCameraProperties& AFCameraComponent::GetCameraProperties() const
{
	return m_cameraProperties;
}
