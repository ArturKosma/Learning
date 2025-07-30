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
		SetActiveCamera(editorCamera->GetCameraComponent());
	}
}

void AFCameraManager::UpdateState()
{
	switch (AFGame::GetGame()->GetControlMode())
	{
	case EAFControlMode::Editor:
	{
		std::shared_ptr<AFCamera> editorCamera = std::dynamic_pointer_cast<AFCamera>(AFGame::GetGame()->GetScene().FindActor("editorCamera0"));
		if (editorCamera)
		{
			AFTimerManager::GetInstance().ClearTimer(m_currentBlend);
			editorCamera->SetLocation(GetActiveCamera()->GetWorldLocation());
			editorCamera->SetRotation(GetActiveCamera()->GetWorldRotationQuat());
			editorCamera->GetMovementComponent()->ResetControlRotation();
			SetActiveCamera(editorCamera->GetCameraComponent());
		}
		break;
	}
	case EAFControlMode::ActionRPG:
	{
		BlendTo(m_playerPawn, 0.3f, EAFInterpolationType::CubicHermite);
		break;
	}
	default:
	{
		break;
	}
	}
}

void AFCameraManager::SetActiveCamera(std::shared_ptr<AFCameraComponent> otherCamera)
{
	m_activeCamera = otherCamera;

	FAFSceneData& sceneData = const_cast<FAFSceneData&>(AFGame::GetGame()->GetScene().GetSceneData());
	sceneData.activeCamera = m_activeCamera;
}

std::shared_ptr<AFCameraComponent> AFCameraManager::GetActiveCamera() const
{
	return m_activeCamera;
}

void AFCameraManager::BlendTo(std::shared_ptr<AFActor> actorWithCamComp, float length,
                              EAFInterpolationType interpType)
{
	if (!actorWithCamComp)
	{
		return;
	}

	std::shared_ptr<AFCameraComponent> targetCameraComp = actorWithCamComp->GetComponentByType<AFCameraComponent>();
	if (!targetCameraComp)
	{
		return;
	}

	std::shared_ptr<AFCamera> tempCamera = std::dynamic_pointer_cast<AFCamera>(AFGame::GetGame()->GetScene().FindActor("tempCamera0"));
	if (!tempCamera)
	{
		return;
	}

	// Move temp camera to our current camera and set it active.
	tempCamera->SetTransform(GetActiveCamera()->GetWorldTransform());
	SetActiveCamera(tempCamera->GetCameraComponent());

	const glm::vec3 startLoc = tempCamera->GetLocation();
	const glm::quat startRot = tempCamera->GetRotationQuat();

	AFTimerManager::GetInstance().ClearTimer(m_currentBlend);

	// Blend to location and rotation of the target.
	// Activate target camera upon reaching.
	m_currentBlend = AFTimerManager::SetAlphaTimer(
	[this, targetCameraComp, tempCamera, startLoc, startRot](float alpha, float timeElapsed)
	{
		tempCamera->SetLocation(glm::mix(startLoc, targetCameraComp->GetWorldLocation(), alpha));
		tempCamera->SetRotation(glm::slerp(startRot, targetCameraComp->GetWorldRotationQuat(), alpha));
	},
	length,
	[this, targetCameraComp]()
	{
		SetActiveCamera(targetCameraComp);
	},
	interpType
	);
}

void AFCameraManager::BlendToStatic(const glm::vec3& targetLocation, float targetPitch, float targetYaw, float length)
{
	if (AFGame::GetGame()->GetControlMode() != EAFControlMode::Editor)
	{
		return;
	}

	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	m_blending = true;

	const glm::vec3& startLocation = weakActiveCamera.lock()->GetLocation();
	const float startPitch = weakActiveCamera.lock()->GetMovementComponent()->GetControlRotation().x;
	const float startYaw = weakActiveCamera.lock()->GetMovementComponent()->GetControlRotation().y;

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
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		weakActiveCamera.lock()->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				yaw * AFTimerManager::GetDeltaTime() * -5.0f,
				0.0f));
	}
}

void AFCameraManager::AddYawStroke(float yaw)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		weakActiveCamera.lock()->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				yaw * AFTimerManager::GetDeltaTime() * 10.0f,
				0.0f));
	}
}

void AFCameraManager::AddPitch(float pitch)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		weakActiveCamera.lock()->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				pitch * AFTimerManager::GetDeltaTime() * -5.0f,
				0.0f,
				0.0f));
	}
}

void AFCameraManager::AddPitchStroke(float pitch)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		weakActiveCamera.lock()->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				pitch * AFTimerManager::GetDeltaTime() * 10.0f,
				0.0f,
				0.0f));
	}
}

void AFCameraManager::ForwardBackward(float axis)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = weakActiveCamera.lock()->GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& movementInputDirection = forward * axis;

		weakActiveCamera.lock()->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::RightLeft(float axis)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = weakActiveCamera.lock()->GetRotationQuat();
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& movementInputDirection = (right * axis);

		weakActiveCamera.lock()->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::UpDown(float axis)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& movementInputDirection = (up * axis);

		weakActiveCamera.lock()->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCameraManager::ZoomStroke(float axis)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	if (!m_blending)
	{
		const glm::quat& cameraRotQuat = weakActiveCamera.lock()->GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& offset = forward * axis * 150.0f * AFTimerManager::GetDeltaTime();

		weakActiveCamera.lock()->GetMovementComponent()->AddOffset(offset);
	}
}

void AFCameraManager::AddCameraSpeedMultiplier(float value)
{
	std::weak_ptr<AFCamera> weakActiveCamera = std::dynamic_pointer_cast<AFCamera>(m_activeCamera->GetOwner().lock());
	if (!weakActiveCamera.lock())
	{
		return;
	}

	weakActiveCamera.lock()->GetMovementComponent()->AddCameraSpeedMultiplier(value);
}
