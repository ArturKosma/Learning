#pragma once
#include "AFMovementComponent.h"

class AFCharacterMovementComponent : public AFMovementComponent
{
public:

	void Tick(float deltaTime) override;
};
