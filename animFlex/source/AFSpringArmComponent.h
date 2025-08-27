#pragma once
#include "AFCameraComponent.h"

class AFSpringArmComponent : public AFSceneComponent
{
public:

	void OnOwnerTransform(glm::mat4 offset) override;

	void Tick(float deltaTime) override;

	void SetArmLength(float newLength);
	void AttachCamera(std::shared_ptr<AFCameraComponent> camera);

private:

	float m_armLength = 200.0f;
	std::shared_ptr<AFCameraComponent> m_attachedCamera = nullptr;

	glm::vec3 m_currentWorldLocation = glm::vec3(0.0f);
	glm::vec3 m_targetWorldLocation = glm::vec3(0.0f);
};
