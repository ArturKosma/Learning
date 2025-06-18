#include "AFPlayerPawn.h"
#include "AFScene.h"

AFPlayerPawn::AFPlayerPawn()
	: AFActor()
{
	m_meshComponent = AFScene::CreateObject<AFSkeletalMeshComponent>();
	AddComponent(m_meshComponent);
}

std::shared_ptr<AFSkeletalMeshComponent> AFPlayerPawn::GetMeshComponent() const
{
	return m_meshComponent;
}
