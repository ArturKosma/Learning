#pragma once
#include "AFActor.h"
#include "AFCameraComponent.h"
#include "AFSkeletalMeshComponent.h"

class AFPlayerPawn : public AFActor
{
public:

	AFPlayerPawn();

	virtual void BeginPlay() override;

	std::shared_ptr<AFCameraComponent> GetCameraComponent() const;
	std::shared_ptr<AFSkeletalMeshComponent> GetMeshComponent() const;

protected:

	std::shared_ptr<AFCameraComponent> m_cameraComponent = nullptr;;
	std::shared_ptr<AFSkeletalMeshComponent> m_meshComponent = nullptr;
};
