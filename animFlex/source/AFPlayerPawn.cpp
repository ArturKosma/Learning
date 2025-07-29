#include "AFPlayerPawn.h"
#include "AFScene.h"

AFPlayerPawn::AFPlayerPawn()
	: AFActor()
{
	m_meshComponent = AFScene::CreateObject<AFSkeletalMeshComponent>();
	AddComponent(m_meshComponent);

	m_cameraComponent = AFScene::CreateObject<AFCameraComponent>();
	AddComponent(m_cameraComponent);
}

std::shared_ptr<AFCameraComponent> AFPlayerPawn::GetCameraComponent() const
{
	return m_cameraComponent;
}

std::shared_ptr<AFSkeletalMeshComponent> AFPlayerPawn::GetMeshComponent() const
{
	return m_meshComponent;
}
