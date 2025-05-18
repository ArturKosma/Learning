#include "AFCameraMovementComponent.h"

#include <glm/common.hpp>

#include "AFActor.h"
#include "AFMath.h"
#include "AFTimerManager.h"

void AFCameraMovementComponent::BlendTo(const glm::vec3& targetLocation, float targetPitch, float targetYaw,
                                        float length)
{
	AFActor* owner = dynamic_cast<AFActor*>(GetOwner());
	if(!owner)
	{
		return;
	}

	const glm::vec3& startLocation = owner->GetLocation();
	const float startPitch = GetControlRotation().x;
	const float startYaw = GetControlRotation().y;

	m_cameraMovementMode = EAFCameraMovementMode::Blending;

	AFTimerManager::SetAlphaTimer([this, owner, startLocation, startPitch, startYaw, targetLocation, targetPitch, targetYaw](float alpha, float timeElapsed)
		{
			owner->SetLocation(glm::mix(startLocation, targetLocation, alpha));
			SetControlRotation(
				glm::vec3(
					AFMath::AngleLerp(startPitch, targetPitch, alpha),
					AFMath::AngleLerp(startYaw, targetYaw, alpha),
					0.0f));
		}, length, [this]()
		{
				m_cameraMovementMode = EAFCameraMovementMode::Normal;
		}, EAFInterpolationType::CubicHermite);
}

void AFCameraMovementComponent::AddControlRotation(const glm::vec3& eulerToAdd)
{
	if(m_cameraMovementMode == EAFCameraMovementMode::Normal)
	{
		AFMovementComponent::AddControlRotation(eulerToAdd);
	}
}

void AFCameraMovementComponent::AddMovementInput(const glm::vec3& movementInput)
{
	if (m_cameraMovementMode == EAFCameraMovementMode::Normal)
	{
		AFMovementComponent::AddMovementInput(movementInput);
	}
}

void AFCameraMovementComponent::AddOffset(const glm::vec3& offset)
{
	if(m_cameraMovementMode == EAFCameraMovementMode::Normal)
	{
		AFMovementComponent::AddOffset(offset);
	}
}

float AFCameraMovementComponent::GetAcceleration() const
{
	return AFMovementComponent::GetAcceleration() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetDeceleration() const
{
	return AFMovementComponent::GetDeceleration() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetMaxSpeed() const
{
	return AFMovementComponent::GetMaxSpeed() * m_cameraSpeedMultiplier;
}

float AFCameraMovementComponent::GetCameraSpeedMultiplier() const
{
	return m_cameraSpeedMultiplier;
}

void AFCameraMovementComponent::AddCameraSpeedMultiplier(float direction)
{
	m_cameraSpeedMultiplier += (direction * 25.0f);

	m_cameraSpeedMultiplier = glm::clamp(m_cameraSpeedMultiplier, 25.0f, 600.0f);
}
