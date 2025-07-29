#pragma once

#include "AFActor.h"
#include "AFCameraComponent.h"
#include "AFCameraMovementComponent.h"

class AFCamera : public AFActor
{
	friend class AFCameraManager;

public:

	AFCamera();

	std::shared_ptr<AFCameraComponent> GetCameraComponent() const;
	std::shared_ptr<AFCameraMovementComponent> GetMovementComponent() const;

protected:

	void OnTransformRecreation() override;

private:

	std::shared_ptr<AFCameraMovementComponent> m_movementComponent = nullptr;
	std::shared_ptr<AFCameraComponent> m_cameraComponent = nullptr;
};
