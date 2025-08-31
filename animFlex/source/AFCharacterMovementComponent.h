#pragma once
#include "AFMovementComponent.h"

class AFCharacterMovementComponent : public AFMovementComponent
{
public:

	float GetAcceleration() const override;
	float GetDeceleration() const override;
	float GetMaxSpeed() const override;

	void Tick(float deltaTime) override;

private:

	float m_previousRootYaw = 0.0f;

	glm::vec3 GetLocationAfterBallPivotRot(float deltaYaw);
};
