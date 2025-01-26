#include "AFGame.h"
#include "AFInput.h"
#include "AFTimerManager.h"
#include "AFCameraManager.h"

#include <glm/glm.hpp>

void AFGame::Init()
{
	if (!m_scene.Init())
	{
		printf("%s\n", "Scene Init() failed.");
		return;
	}

	AFCamera* initCamera = new AFCamera();
	m_scene.AddActor(initCamera);

	m_cameraManager = new AFCameraManager();
	m_cameraManager->SetActiveCamera(initCamera);
}

void AFGame::Tick(float deltaTime)
{
	for(AFActor* actor : m_scene.GetSceneData().sceneActors)
	{
		actor->Tick(deltaTime);
	}
}

AFCameraManager* AFGame::GetCameraManager() const
{
	return m_cameraManager;
}

const AFScene& AFGame::GetScene()
{
	return m_scene;
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}

void AFGame::OnInput(int pressState)
{
	/*if(pressState == 1)
	{
		SetTestState(!m_testState);
	}*/
}

void AFGame::OnCursorPosUpdate(double deltaX, double deltaY)
{
	if(AFInput::GetMouseLock())
	{
		constexpr float cameraRotStrength = 5.0f;

		m_camera.AddRotation(glm::vec3(static_cast<float>(deltaY) * AFTimerManager::GetDeltaTime() * cameraRotStrength,
			static_cast<float>(deltaX) * AFTimerManager::GetDeltaTime() * cameraRotStrength, 0.0f));
	}
}

void AFGame::OnAxisInput(const std::map<unsigned int, float>& axisInputs)
{
	if (AFInput::GetMouseLock())
	{
		const float forwardBackward = axisInputs.at(0);
		const float leftRight = axisInputs.at(1);
		const float upDown = axisInputs.at(2);

		const glm::quat& cameraRotQuat = m_camera.GetRotation();

		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& up = cameraRotQuat * glm::vec3(0.0f, 1.0f, 0.0f);

		const glm::vec3& offsetToAdd = (forward * forwardBackward) +
			(right * leftRight) +
			(up * upDown);

		m_camera.AddMovementInput(offsetToAdd);
	}
}

void AFGame::OnScrollUpdate(double deltaX, double deltaY)
{
	m_camera.UpdateCameraSpeed(deltaY);
}

void AFGame::SetTestState(bool newTestState)
{
	m_testState = newTestState;
}
