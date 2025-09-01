#include "AFPlayerPawn.h"
#include "AFScene.h"

AFPlayerPawn::AFPlayerPawn()
	: AFActor()
{
	m_characterMovementComponent = AFScene::CreateObject<AFCharacterMovementComponent>();
	AddComponent(m_characterMovementComponent);

	m_meshComponent = AFScene::CreateObject<AFSkeletalMeshComponent>();
	AddComponent(m_meshComponent);

	m_cameraComponent = AFScene::CreateObject<AFCameraComponent>();
	m_cameraComponent->SetDisplayName("heroCamera");
	AddComponent(m_cameraComponent);

	m_springArmComponent = AFScene::CreateObject<AFSpringArmComponent>();
	AddComponent(m_springArmComponent);

	m_springArmComponent->AttachCamera(m_cameraComponent);
	m_springArmComponent->AddLocalOffsetLocation(glm::vec3(0.0f, 130.0f, 0.0f));
	m_springArmComponent->SetArmLength(270.0f);
}

void AFPlayerPawn::BeginPlay()
{
	AFActor::BeginPlay();
}

void AFPlayerPawn::Tick(float deltaTime)
{
	AFActor::Tick(deltaTime);

	//printf("pawn pos: %f, %f, %f\n", GetLocation().x, GetLocation().y, GetLocation().z);
	//printf("pawn rot: %f, %f, %f\n", GetRotation().x, GetRotation().y, GetRotation().z);
	//printf("cam pos: %f, %f, %f\n", m_cameraComponent->GetWorldLocation().x, m_cameraComponent->GetWorldLocation().y, m_cameraComponent->GetWorldLocation().z);
	//printf("cam rot: %f, %f, %f\n", m_cameraComponent->GetWorldRotation().x, m_cameraComponent->GetWorldRotation().y, m_cameraComponent->GetWorldRotation().z);
}

std::shared_ptr<AFCameraComponent> AFPlayerPawn::GetCameraComponent() const
{
	return m_cameraComponent;
}

std::shared_ptr<AFSkeletalMeshComponent> AFPlayerPawn::GetMeshComponent() const
{
	return m_meshComponent;
}

std::shared_ptr<AFSpringArmComponent> AFPlayerPawn::GetSpringArmComponent() const
{
	return m_springArmComponent;
}

std::shared_ptr<AFCharacterMovementComponent> AFPlayerPawn::GetCharacterMovementComponent() const
{
	return m_characterMovementComponent;
}
