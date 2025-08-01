#pragma once

#include "AFMovementComponent.h"
#include "AFStructs.h"

class AFCameraMovementComponent : public AFMovementComponent
{
public:
	void SetControlRotation(const glm::vec3& newControlRotation) override;

	void AddControlRotation(const glm::vec3& eulerToAdd) override;
	void AddMovementInput(const glm::vec3& movementInput) override;
	void AddOffset(const glm::vec3& offset) override;

	float GetAcceleration() const override;
	float GetDeceleration() const override;
	float GetMaxSpeed() const override;

	float GetCameraSpeedMultiplier() const;

	void AddCameraSpeedMultiplier(float direction);

	void ResetControlRotation();

private:

	float m_cameraSpeedMultiplier = 100.0f;
};
