#include "AFGame.h"
#include "AFCamera.h"
#include "AFInput.h"
#include "AFTimerManager.h"
#include "AFUtility.h"

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

	//const unsigned int id = 4228;
	/*const glm::ivec4 idBytes = glm::ivec4(
		(id >> 0) & 0xFF,
		(id >> 8) & 0xFF,
		(id >> 16) & 0xFF,
		(id >> 24) & 0xFF
	);
	const glm::vec4 idBytesNormalized = glm::vec4(idBytes) / 255.0f;

	const glm::uvec4 idBytesUnpack = glm::uvec4(idBytesNormalized * 255.0f + 0.5f);

	const unsigned int reconstructId = 
		(idBytesUnpack.r << 0) | 
		(idBytesUnpack.g << 8) |
		(idBytesUnpack.b << 16) |
		(idBytesUnpack.a << 24);*/

	//printf("reconstructId: %d\n", AFUtility::UnpackID(AFUtility::PackID(id)));
	//printf("mid product: %f, %f, %f, %f", idBytesNormalized.r, idBytesNormalized.g, idBytesNormalized.b, idBytesNormalized.a);

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
