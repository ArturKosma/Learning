#pragma once

#include "AFActor.h"
#include "AFMovementComponent.h"
#include "AFCameraComponent.h"

class AFCamera : public AFActor
{
public:

	AFCamera();

	AFCameraComponent* GetCameraComponent() const;

private:

	AFMovementComponent* m_movementComponent = nullptr;
	AFCameraComponent* m_cameraComponent = nullptr;
};
