#include "AFCamera.h"

#include "AFInput.h"
#include "AFMath.h"
#include "AFMovementComponent.h"
#include "AFTimerManager.h"
#include "AFScene.h"

AFCamera::AFCamera()
{
	m_movementComponent = AFScene::CreateObject<AFCameraMovementComponent>();
	AddComponent(m_movementComponent);

	m_cameraComponent = AFScene::CreateObject<AFCameraComponent>();
	AddComponent(m_cameraComponent);

	BindInputs();
}

std::shared_ptr<AFCameraComponent> AFCamera::GetCameraComponent() const
{
	return m_cameraComponent;
}

std::shared_ptr<AFCameraMovementComponent> AFCamera::GetMovementComponent() const
{
	return m_movementComponent;
}

void AFCamera::OnTransformRecreation()
{
	m_location.y = glm::clamp(m_location.y, 30.0f, 10000.0f);

	AFActor::OnTransformRecreation();
}

void AFCamera::BindInputs()
{
	AFInput::BindAxis("FreeView_CameraYaw", [this](float deltaX) {Input_FreeView_CameraYaw(deltaX); });
	AFInput::BindAxis("FreeView_CameraPitch", [this](float deltaY) {Input_FreeView_CameraPitch(deltaY); });

	AFInput::BindAxis("FreeView_CameraYaw_Stroke", [this](float deltaX) {Input_FreeView_CameraYaw_Stroke(deltaX); });
	AFInput::BindAxis("FreeView_CameraPitch_Stroke", [this](float deltaY) {Input_FreeView_CameraPitch_Stroke(deltaY); });

	AFInput::BindAxis("FreeView_ForwardBackward", [this](float axis) {Input_FreeView_ForwardBackward(axis); });
	AFInput::BindAxis("FreeView_RightLeft", [this](float axis) {Input_FreeView_RightLeft(axis); });
	AFInput::BindAxis("FreeView_UpDown", [this](float axis) {Input_FreeView_UpDown(axis); });

	AFInput::BindAxis("FreeView_Zoom_Stroke", [this](float axis) {Input_FreeView_Zoom_Stroke(axis); });

	AFInput::BindAction("CameraAddSpeed", [this] {if(AFInput::GetFreeViewMode()) m_movementComponent->AddCameraSpeedMultiplier(1.0f); }, EAFKeyAction::Pressed);
	AFInput::BindAction("CameraLowerSpeed", [this] {if (AFInput::GetFreeViewMode()) m_movementComponent->AddCameraSpeedMultiplier(-1.0f); }, EAFKeyAction::Pressed);
}

void AFCamera::Input_FreeView_CameraYaw(float delta)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				delta * AFTimerManager::GetDeltaTime() * m_cameraRotStrength * -1.0f,
				0.0f));
	}
}

void AFCamera::Input_FreeView_CameraPitch(float delta)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		GetMovementComponent()->AddControlRotation(
			glm::vec3(
				delta * AFTimerManager::GetDeltaTime() * m_cameraRotStrength * -1.0f,
				0.0f,
				0.0f));
	}
}

void AFCamera::Input_FreeView_CameraYaw_Stroke(float delta)
{
	GetMovementComponent()->AddControlRotation(
		glm::vec3(
			0.0f,
			delta * AFTimerManager::GetDeltaTime() * m_cameraRotStrength_stroke,
			0.0f));
}

void AFCamera::Input_FreeView_CameraPitch_Stroke(float delta)
{
	GetMovementComponent()->AddControlRotation(
		glm::vec3(
			delta * AFTimerManager::GetDeltaTime() * m_cameraRotStrength_stroke,
			0.0f,
			0.0f));
}

void AFCamera::Input_FreeView_ForwardBackward(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		const glm::quat& cameraRotQuat = GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& movementInputDirection = forward * axis;

		GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCamera::Input_FreeView_RightLeft(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		const glm::quat& cameraRotQuat = GetRotationQuat();
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& movementInputDirection = (right * axis);

		GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCamera::Input_FreeView_UpDown(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& movementInputDirection = (up * axis);

		GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFCamera::Input_FreeView_Zoom_Stroke(float axis)
{
	const glm::quat& cameraRotQuat = GetRotationQuat();
	const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec3& offset = forward * axis * 150.0f * AFTimerManager::GetDeltaTime();

	GetMovementComponent()->AddOffset(offset);
}
