#include "AFCharacterMovementComponent.h"
#include "AFActor.h"
#include "AFAnimState.h"
#include "AFEvaluator.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"

float AFCharacterMovementComponent::GetAcceleration() const
{
	return 800.0f;
}

float AFCharacterMovementComponent::GetDeceleration() const
{
	return 1500.0f;
}

float AFCharacterMovementComponent::GetMaxSpeed() const
{
	return 500.0f;
}

void AFCharacterMovementComponent::Tick(float deltaTime)
{
	const bool paused = AFEvaluator::Get().GetAnimPaused();
	const float playrate = AFEvaluator::Get().GetAnimPlayrate();
	deltaTime = paused ? 0.0f : deltaTime * playrate;

	AFMovementComponent::Tick(deltaTime);

	std::shared_ptr<AFPlayerPawn> ownerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());
	if (!ownerPawn)
	{
		return;
	}

	ownerPawn->SetRotation({ 0.0f, -m_controlYaw, 0.0f });
}
