#include "AFSpringArmComponent.h"
#include "AFPlayerPawn.h"
#include "AFMath.h"

// @todo Allow any attachment to spring arm, which would result in proper transform of the attached object.

void AFSpringArmComponent::Tick(float deltaTime)
{
	std::shared_ptr<AFPlayerPawn> playerPawn = std::dynamic_pointer_cast<AFPlayerPawn>(GetOwner().lock());

	if (m_attachedCamera && playerPawn)
	{
		// Rotate the camera 180d on yaw, otherwise it will away from the pawn.
		const glm::mat4 lookBack = glm::mat4_cast(glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

		// Construct a mat4 from control rotation.
		glm::vec3 controlEuler = playerPawn->GetCharacterMovementComponent()->GetControlRotation();
		controlEuler = glm::vec3(controlEuler.x * -1.0f, controlEuler.y * -1.0f, 0.0f);
		const glm::mat4 controlRot = glm::mat4_cast(glm::quat(glm::radians(controlEuler)));

		// Offset by the spring arm's length.
		const glm::mat4 springOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -m_armLength));

		// Create the orbit transform.
		const glm::mat4 orbit = controlRot * springOffset * lookBack;

		// Don't transform the spring arm, update the local transform of attached camera.
		m_attachedCamera->SetLocalTransform(GetLocalTransform() * orbit);

		// Collision with the ground.
		glm::vec3 worldLoc = m_attachedCamera->GetWorldLocation();
		const float offsetY = 5.0f;
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
