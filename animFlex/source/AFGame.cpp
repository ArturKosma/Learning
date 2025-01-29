#include "AFGame.h"
#include "AFCamera.h"
#include "AFInput.h"
#include "AFMath.h"
#include "AFTimerManager.h"

bool AFGame::Init()
{
	if (!m_scene.Init())
	{
		printf("%s\n", "Scene Init() failed.");
		return false;
	}

	// Create default camera.
	AFCamera* initCamera = new AFCamera();
	m_scene.AddActor(initCamera);
	initCamera->SetLocation({ 0.0f, 2.0f, 4.0f });
	m_scene.SetActiveCamera(initCamera);

	BindInputs();

	return true;
}

void AFGame::Tick(float deltaTime)
{
	for(AFActor* actor : m_scene.GetSceneData().sceneActors)
	{
		actor->Tick(deltaTime);
	}
}

const AFScene& AFGame::GetScene()
{
	return m_scene;
}

void AFGame::BindInputs()
{
	AFInput::GetInstance().BindAxis("FreeView_CameraYaw", [this](float deltaX) {Input_FreeView_CameraYaw(deltaX); });
	AFInput::GetInstance().BindAxis("FreeView_CameraPitch", [this](float deltaY) {Input_FreeView_CameraPitch(deltaY); });

	AFInput::GetInstance().BindAxis("FreeView_ForwardBackward", [this](float axis) {Input_FreeView_ForwardBackward(axis); });
	AFInput::GetInstance().BindAxis("FreeView_RightLeft", [this](float axis) {Input_FreeView_RightLeft(axis); });
	AFInput::GetInstance().BindAxis("FreeView_UpDown", [this](float axis) {Input_FreeView_UpDown(axis); });
}

void AFGame::Input_FreeView_CameraYaw(float delta)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		constexpr float cameraRotStrength = 5.0f;
		AFCamera* activeCamera = m_scene.GetActiveCamera();
		activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				0.0f,
				static_cast<float>(delta) * AFTimerManager::GetDeltaTime() * cameraRotStrength * -1.0f,
				0.0f));
	}
}

void AFGame::Input_FreeView_CameraPitch(float delta)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		constexpr float cameraRotStrength = 5.0f;
		AFCamera* activeCamera = m_scene.GetActiveCamera();
		activeCamera->GetMovementComponent()->AddControlRotation(
			glm::vec3(
				static_cast<float>(delta) * AFTimerManager::GetDeltaTime() * cameraRotStrength * -1.0f,
				0.0f,
				0.0f));
	}
}

void AFGame::Input_FreeView_ForwardBackward(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		AFCamera* activeCamera = GetScene().GetActiveCamera();

		const glm::quat& cameraRotQuat = activeCamera->GetRotationQuat();
		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& movementInputDirection = forward * axis;

		activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFGame::Input_FreeView_RightLeft(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		AFCamera* activeCamera = GetScene().GetActiveCamera();

		const glm::quat& cameraRotQuat = activeCamera->GetRotationQuat();
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& movementInputDirection = (right * axis);

		activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

void AFGame::Input_FreeView_UpDown(float axis)
{
	if (AFInput::GetInstance().GetFreeViewMode())
	{
		AFCamera* activeCamera = GetScene().GetActiveCamera();

		const glm::quat& cameraRotQuat = activeCamera->GetRotationQuat();
		const glm::vec3& up = cameraRotQuat * glm::vec3(0.0f, 1.0f, 0.0f);
		const glm::vec3& movementInputDirection = (up * axis);

		activeCamera->GetMovementComponent()->AddMovementInput(movementInputDirection);
	}
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}

/*void AFGame::OnScrollUpdate(double deltaX, double deltaY)
{
	//m_camera.UpdateCameraSpeed(deltaY);
}*/
