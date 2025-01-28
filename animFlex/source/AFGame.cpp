#include "AFGame.h"
#include "AFCamera.h"
#include "AFInput.h"

bool AFGame::Init()
{
	if (!m_scene.Init())
	{
		printf("%s\n", "Scene Init() failed.");
		return false;
	}

	AFCamera* initCamera = new AFCamera();
	m_scene.AddActor(initCamera);
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
	AFInput::BindAction("FreeViewMode", [this] {Input_FreeViewMode_Pressed(); }, EAFKeyAction::Pressed);
	AFInput::BindAction("FreeViewMode", [this] {Input_FreeViewMode_Released(); }, EAFKeyAction::Released);

	AFInput::BindAxis("FreeView_CameraYaw", [this](float deltaX) {Input_FreeView_CameraYaw(deltaX); });
	AFInput::BindAxis("FreeView_CameraPitch", [this](float deltaY) {Input_FreeView_CameraPitch(deltaY); });

	AFInput::BindAxis("FreeView_ForwardBackward", [this](float axis) {Input_FreeView_ForwardBackward(axis); });
	AFInput::BindAxis("FreeView_RightLeft", [this](float axis) {Input_FreeView_RightLeft(axis); });
	AFInput::BindAxis("FreeView_UpDown", [this](float axis) {Input_FreeView_UpDown(axis); });
}

void AFGame::Input_FreeViewMode_Pressed()
{
	m_freeViewMode = true;
}

void AFGame::Input_FreeViewMode_Released()
{
	m_freeViewMode = false;
}

void AFGame::Input_FreeView_CameraYaw(float deltaX)
{
	//printf("%f\n", deltaX);
}

void AFGame::Input_FreeView_CameraPitch(float deltaY)
{
}

void AFGame::Input_FreeView_ForwardBackward(float axis)
{
	
}

void AFGame::Input_FreeView_RightLeft(float axis)
{
}

void AFGame::Input_FreeView_UpDown(float axis)
{
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}

/*void AFGame::OnCursorPosUpdate(double deltaX, double deltaY)
{
	if(AFInput::GetMouseLock())
	{
		constexpr float cameraRotStrength = 5.0f;

		m_camera.AddRotation(glm::vec3(static_cast<float>(deltaY) * AFTimerManager::GetDeltaTime() * cameraRotStrength,
			static_cast<float>(deltaX) * AFTimerManager::GetDeltaTime() * cameraRotStrength, 0.0f));
	}
}*/

/*void AFGame::OnAxisInput(const std::map<unsigned int, float>& axisInputs)
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
}*/

/*void AFGame::OnScrollUpdate(double deltaX, double deltaY)
{
	//m_camera.UpdateCameraSpeed(deltaY);
}*/
