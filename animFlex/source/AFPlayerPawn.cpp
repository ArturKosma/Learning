#include "AFPlayerPawn.h"
#include "AFScene.h"

AFPlayerPawn::AFPlayerPawn()
	: AFActor()
{
	m_meshComponent = AFScene::CreateObject<AFSkeletalMeshComponent>();
	AddComponent(m_meshComponent);

	m_cameraComponent = AFScene::CreateObject<AFCameraComponent>();
	m_cameraComponent->SetDisplayName("heroCamera");
	m_cameraComponent->SetLocalLocation({0.0f, 100.0f, -200.0f });
	m_cameraComponent->SetLocalRotation({ 0.0f, 180.0f, 0.0f });
	AddComponent(m_cameraComponent);
}

void AFPlayerPawn::BeginPlay()
{
	AFActor::BeginPlay();
}

std::shared_ptr<AFCameraComponent> AFPlayerPawn::GetCameraComponent() const
{
	return m_cameraComponent;
}

std::shared_ptr<AFSkeletalMeshComponent> AFPlayerPawn::GetMeshComponent() const
{
	return m_meshComponent;
}
