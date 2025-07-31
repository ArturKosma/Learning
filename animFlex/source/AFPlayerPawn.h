#pragma once
#include "AFActor.h"
#include "AFCameraComponent.h"
#include "AFCharacterMovementComponent.h"
#include "AFSkeletalMeshComponent.h"
#include "AFSpringArmComponent.h"

class AFPlayerPawn : public AFActor
{
public:

	AFPlayerPawn();

	virtual void BeginPlay() override;

	std::shared_ptr<AFCameraComponent> GetCameraComponent() const;
	std::shared_ptr<AFSkeletalMeshComponent> GetMeshComponent() const;
	std::shared_ptr<AFSpringArmComponent> GetSpringArmComponent() const;
	std::shared_ptr<AFCharacterMovementComponent> GetCharacterMovementComponent() const;

protected:

	std::shared_ptr<AFCameraComponent> m_cameraComponent = nullptr;;
	std::shared_ptr<AFSkeletalMeshComponent> m_meshComponent = nullptr;
	std::shared_ptr<AFSpringArmComponent> m_springArmComponent = nullptr;
	std::shared_ptr<AFCharacterMovementComponent> m_characterMovementComponent = nullptr;
};
