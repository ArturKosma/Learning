#include "AFCameraManager.h"

#include "AFGame.h"
#include "AFMath.h"
#include "AFPlayerPawn.h"
#include "AFTimerManager.h"

void AFCameraManager::Init(const AFScene& scene)
{
	m_playerPawn = scene.GetPlayerPawn();
}

void AFCameraManager::BeginPlay()
{
	std::shared_ptr<AFCamera> editorCamera = std::dynamic_pointer_cast<AFCamera>(AFGame::GetGame()->GetScene().FindActor("editorCamera0"));
	if (editorCamera)
	{
		SetActiveCamera(editorCamera);
	}
}

void AFCameraManager::UpdateState()
{
	switch (AFGame::GetGame()->GetControlMode())
	{
	case EAFControlMode::Editor:
	{
		break;
	}
	case EAFControlMode::ActionRPG:
	{
		BlendTo(m_playerPawn->GetCameraComponent(), 1.0f, EAFInterpolationType::CubicHermite);
		break;
	}
	default:
	{
		break;
	}
	}
}

void AFCameraManager::SetActiveCamera(std::shared_ptr<AFCamera> otherCamera)
{
	m_activeCamera = otherCamera;

	FAFSceneData& sceneData = const_cast<FAFSceneData&>(AFGame::GetGame()->GetScene().GetSceneData());
	sceneData.activeCamera = m_activeCamera;
}

std::shared_ptr<AFCamera> AFCameraManager::GetActiveCamera() const
{
	return m_activeCamera;
}

void AFCameraManager::BlendTo(std::shared_ptr<AFCameraComponent> otherCamera, float length,
                              EAFInterpolationType interpType)
{
	// Create temporary camera for the blend duration.
}

void AFCameraManager::BlendToStatic(const glm::vec3& targetLocation, float targetPitch, float targetYaw, float length)
{
	if (AFGame::GetGame()->GetControlMode() != EAFControlMode::Editor)
	{
		return;
	}

	std::weak_ptr<AFCamera> weakActiveCamera = m_activeCamera;
	if (!weakActiveCamera.lock())
	{
		return;
	}

	m_blending = true;

	const glm::vec3& startLocation = m_activeCamera->GetLocation();
	const float startPitch = m_activeCamera->GetMovementComponent()->GetControlRotation().x;
	const float startYaw = m_activeCamera->GetMovementComponent()->GetControlRotation().y;

	AFTimerManager::SetAlphaTimer([this, weakActiveCamera, startLocation, startPitch, startYaw, targetLocation, targetPitch, targetYaw]
	(float alpha, float timeElapsed)
		{
			weakActiveCamera.lock()->SetLocation(glm::mix(startLocation, targetLocation, alpha));
			weakActiveCamera.lock()->GetMovementComponent()->SetControlRotation(
				glm::vec3(
					AFMath::AngleLerp(startPitch, targetPitch, alpha),
					AFMath::AngleLerp(startYaw, targetYaw, alpha),
					0.0f));
		}, length, [this]()
		{
			m_blending = false;
		}, EAFInterpolationType::CubicHermite);
}

void AFCameraManager::AddYaw(float yaw)
{
	if (!m_blending)
	{
		m_activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				yaw * AFTimerManager::GetDeltaTime() * -5.0f,
				0.0f));
	}
}

void AFCameraManager::AddYawStroke(float yaw)
{
	if (!m_blending)
	{
		m_activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				yaw * AFTimerManager::GetDeltaTime() * 10.0f,
				0.0f));
	}
}

void AFCameraManager::AddPitch(float pitch)
{
	if (!m_blending)
	{
		m_activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				pitch * AFTimerManager::GetDeltaTime() * -5.0f,
				0.0f,
				0.0f));
	}
}

void AFCameraManager::AddPitchStroke(float pitch)
{
	if (!m_blending)
	{
		m_activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				pitch * AFTimerManager::GetDeltaTime() * 10.0f,
				0.0f,
				0.0f));
	}
}

void AFCameraManager::ForwardBackward(float axis)
{
	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = m_activeCamera->GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& movementInputDirection = forward * axis;

		m_activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::RightLeft(float axis)
{
	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = m_activeCamera->GetRotationQuat();
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& movementInputDirection = (right * axis);

		m_activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::UpDown(float axis)
{
	if (!m_blending)
	{
		const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& movementInputDirection = (up * axis);

		m_activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::ZoomStroke(float axis)
{
	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = m_activeCamera->GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& offset = forward * axis * 150.0f * AFTimerManager::GetDeltaTime();

		m_activeCamera->GetMovementComponent()->AddOffset(offset);
	}
}

void AFCameraManager::AddCameraSpeedMultiplier(float value)
{
	m_activeCamera->GetMovementComponent()->AddCameraSpeedMultiplier(value);
}
