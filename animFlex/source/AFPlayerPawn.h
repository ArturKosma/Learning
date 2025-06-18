#pragma once
#include "AFActor.h"
#include "AFSkeletalMeshComponent.h"

class AFPlayerPawn : public AFActor
{
public:

	AFPlayerPawn();

	std::shared_ptr<AFSkeletalMeshComponent> GetMeshComponent() const;

protected:

	std::shared_ptr<AFSkeletalMeshComponent> m_meshComponent = nullptr;
};
