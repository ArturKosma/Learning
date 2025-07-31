#pragma once
#include "AFCameraComponent.h"

class AFSpringArmComponent : public AFSceneComponent
{
public:

	void Tick(float deltaTime) override;

	void SetArmLength(float newLength);
	void AttachCamera(std::shared_ptr<AFCameraComponent> camera);

private:

	float m_armLength = 200.0f;
	std::shared_ptr<AFCameraComponent> m_attachedCamera = nullptr;

};
