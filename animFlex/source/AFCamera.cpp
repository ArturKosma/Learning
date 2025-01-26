#include "AFCamera.h"
#include "AFMovementComponent.h"

AFCamera::AFCamera()
{
	m_movementComponent = new AFMovementComponent();
	AddComponent(m_movementComponent);

	m_cameraComponent = new AFCameraComponent();
	AddComponent(m_cameraComponent);
}

AFCameraComponent* AFCamera::GetCameraComponent() const
{
	return m_cameraComponent;
}
