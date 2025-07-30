#include "AFCamera.h"

#include "AFInput.h"
#include "AFMath.h"
#include "AFTimerManager.h"
#include "AFScene.h"

AFCamera::AFCamera()
{
	m_movementComponent = AFScene::CreateObject<AFCameraMovementComponent>();
	AddComponent(m_movementComponent);

	m_cameraComponent = AFScene::CreateObject<AFCameraComponent>();
	AddComponent(m_cameraComponent);
}

void AFCamera::BeginPlay()
{
	AFActor::BeginPlay();
}

std::shared_ptr<AFCameraComponent> AFCamera::GetCameraComponent() const
{
	return m_cameraComponent;
}

std::shared_ptr<AFCameraMovementComponent> AFCamera::GetMovementComponent() const
{
	return m_movementComponent;
}

void AFCamera::OnTransformRecreation()
{
	m_location.y = glm::clamp(m_location.y, 30.0f, 10000.0f);

	AFActor::OnTransformRecreation();
}
