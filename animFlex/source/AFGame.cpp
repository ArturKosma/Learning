#include "AFGame.h"
#include "AFCamera.h"
#include "AFInput.h"
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
	initCamera->SetLocation({ 0.0f, 200.0f, 400.0f });
	m_scene.SetActiveCamera(initCamera);

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

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}
