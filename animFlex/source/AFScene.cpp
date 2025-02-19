#include "AFScene.h"

#include "AFBackgroundComponent.h"
#include "AFBoxComponent.h"
#include "AFGridComponent.h"
#include "AFStaticMeshComponent.h"
#include "AFUtility.h"

void AFScene::AddActor(AFActor* newActor)
{
	if(!newActor)
	{
		return;
	}

	// Upon adding new actor to the scene, load all of his renderable assets.
	for(AFComponent* comp : newActor->GetComponents())
	{
		AFRenderComponent* const renderComp = dynamic_cast<AFRenderComponent*>(comp);
		if(!renderComp)
		{
			continue;
		}
		renderComp->Load();
		m_sceneData.vertexCount += renderComp->GetVertexCount();
	}

	m_sceneData.sceneActors.push_back(newActor);
}

void AFScene::SetActiveCamera(AFCamera* newActiveCamera)
{
	m_sceneData.activeCamera = newActiveCamera;
}

AFCamera* AFScene::GetActiveCamera() const
{
	return m_sceneData.activeCamera;
}

const AFSceneData& AFScene::GetSceneData() const
{
	return m_sceneData;
}

bool AFScene::Init()
{
	// Reserve some space for the scene.
	m_sceneData.sceneActors.reserve(20);

	CreateDefaultSceneActors();

	return true;
}

void AFScene::CreateDefaultSceneActors()
{
	// Create background.
	AFActor* background = new AFActor();
	AFBackgroundComponent* backgroundMesh = new AFBackgroundComponent();
	background->AddComponent(backgroundMesh);
	background->SetDisplayName("background");
	AddActor(background);

	// Create a grid.
	AFActor* gridActor = new AFActor();
	gridActor->SetDisplayName("grid");
	AFGridComponent* gridComponent = new AFGridComponent();
	gridActor->AddComponent(gridComponent);
	AddActor(gridActor);

	// Create test boxes.
	AFActor* testBoxActor0 = new AFActor();
	AFActor* testBoxActor1 = new AFActor();
	AFActor* testBoxActor2 = new AFActor();
	testBoxActor0->SetDisplayName("box0");
	testBoxActor1->SetDisplayName("box1");
	testBoxActor1->SetDisplayName("box2");
	AFBoxComponent* testBox0 = new AFBoxComponent();
	testBox0->SetTexture("content/textures/crate2.png");
	testBox0->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	AFBoxComponent* testBox1 = new AFBoxComponent();
	testBox1->SetTexture("content/textures/crate2.png");
	testBox1->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	AFBoxComponent* testBox2 = new AFBoxComponent();
	testBox2->SetTexture("content/textures/crate2.png");
	testBox2->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	testBoxActor0->AddComponent(testBox0);
	testBoxActor1->AddComponent(testBox1);
	testBoxActor2->AddComponent(testBox2);
	AddActor(testBoxActor0);
	AddActor(testBoxActor1);
	AddActor(testBoxActor2);
	testBoxActor0->AddOffsetLocation({ 0.0f, 0.5f, 0.0f });
	testBoxActor1->AddOffsetLocation({ 3.0f, 0.5f, 0.0f });
	testBoxActor1->AddOffsetRotation({ 0.0f, 15.0f, 0.0f });
	testBoxActor2->AddOffsetLocation({ -3.5f, 0.5f, 0.5f });
}

AFScene::AFScene()
{

}

AFScene::~AFScene()
{

}
