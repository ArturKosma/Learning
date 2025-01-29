#pragma once

#include "AFActor.h"
#include "AFMovementComponent.h"
#include "AFCameraComponent.h"

class AFCamera : public AFActor
{
public:

	AFCamera();

	AFCameraComponent* GetCameraComponent() const;
	AFMovementComponent* GetMovementComponent() const;

private:

	AFMovementComponent* m_movementComponent = nullptr;
	AFCameraComponent* m_cameraComponent = nullptr;
};
