#pragma once

#include "AFMovementComponent.h"

class AFCameraMovementComponent : public AFMovementComponent
{
public:

	float GetAcceleration() const override;
	float GetDeceleration() const override;
	float GetMaxSpeed() const override;

	float GetCameraSpeedMultiplier() const;

	void AddCameraSpeedMultiplier(float direction);

private:

	float m_cameraSpeedMultiplier = 3.5f;
};
