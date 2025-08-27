#include "AFSpringArmComponent.h"
#include "AFPlayerPawn.h"
#include "AFMath.h"

// @todo Allow any attachment to spring arm, which would result in proper transform of the attached object automatically.

void AFSpringArmComponent::OnOwnerTransform(glm::mat4 offset)
{
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;

	glm::decompose(offset, scale, rotation, translation, skew, perspective);

	const glm::vec3 eulerOffset = glm::degrees(glm::eulerAngles(rotation));

	// Counter the actor's rotation change by applying local rotation in opposite direction.
	AddLocalOffsetRotation(eulerOffset * -1.0f);
}

void AFSpringArmComponent::Tick(float deltaTime)
{
	std::shared_ptr<AFPlayerPawn> playerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());

	if (m_attachedCamera && playerPawn)
	{
		// Cache new target world location.
		m_targetWorldLocation = playerPawn->GetLocation() + glm::vec3(-8.0f, 117.0f, 0.0f);

		// Keep lerping to the target world location.
		const float interpSpeed = AFMath::MapRangeClamped(glm::length(m_targetWorldLocation - m_currentWorldLocation), 0.0f,100.0f, 0.1f, 650.0f);
		m_currentWorldLocation = AFMath::VInterpTo(m_currentWorldLocation, m_targetWorldLocation, interpSpeed, deltaTime);

		// Set world location.
		//const glm::vec3& loc = GetWorldLocation();
		//printf("%f, %f, %f\n", loc.x, loc.y, loc.z);
		SetWorldLocation(m_currentWorldLocation);

		// Rotate the camera 180d on yaw, otherwise it will look away from the pawn.
		const glm::mat4 lookBack = glm::mat4_cast(glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		// Construct a mat4 from control rotation.
		glm::vec3 controlEuler = playerPawn->GetCharacterMovementComponent()->GetControlRotation();
		controlEuler = glm::vec3(controlEuler.x * -1.0f, controlEuler.y * -1.0f, 0.0f);
		const glm::mat4 controlRot = glm::mat4_cast(glm::quat(glm::radians(controlEuler)));

		// Offset by the spring arm's length.
		const glm::mat4 springOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_armLength));

		// Create the orbit transform.
		const glm::mat4 orbit = controlRot * springOffset * lookBack;

		// Don't transform the spring arm, update the local transform of attached camera instead.
		m_attachedCamera->SetLocalTransform(GetLocalTransform() * orbit);

		// Collision with the ground.
		glm::vec3 worldLoc = m_attachedCamera->GetWorldLocation();
		const float offsetY = 30.0f;
		if (worldLoc.y < offsetY)
		{
			float t = (offsetY - GetWorldLocation().y) / (worldLoc.y - GetWorldLocation().y);

			worldLoc = GetWorldLocation() + t * (worldLoc - GetWorldLocation());
		}
		m_attachedCamera->SetWorldLocation(worldLoc);
	}
}

void AFSpringArmComponent::SetArmLength(float newLength)
{
	m_armLength = newLength;
}

void AFSpringArmComponent::AttachCamera(std::shared_ptr<AFCameraComponent> camera)
{
	m_attachedCamera = camera;
}
