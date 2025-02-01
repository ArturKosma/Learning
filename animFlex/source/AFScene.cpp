#include "AFScene.h"

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
		m_sceneData.sceneTriangles += renderComp->GetTrianglesNum();
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
	// Create a grid.

	AFActor* gridActor = new AFActor();
	gridActor->SetDisplayName("grid");

	AFGridComponent* gridComponent = new AFGridComponent();
	gridActor->AddComponent(gridComponent);

	AddActor(gridActor);

	// Create test box.

	AFMesh testBoxMesh = AFMesh();
	testBoxMesh.vertices.resize(36);

	// Triangles (model space).

	// Front square.
	testBoxMesh.vertices[0].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[1].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[2].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	testBoxMesh.vertices[3].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[4].position = glm::vec3(0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[5].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Behind square.
	testBoxMesh.vertices[6].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[7].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[8].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	//
	testBoxMesh.vertices[9].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[10].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[11].position = glm::vec3(0.5f, 0.5f, -0.5f);
	// Upper square.
	testBoxMesh.vertices[12].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[13].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[14].position = glm::vec3(0.5f, 0.5f, 0.5f);
	//
	testBoxMesh.vertices[15].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[16].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[17].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	// Lower square.
	testBoxMesh.vertices[18].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[19].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[20].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	//
	testBoxMesh.vertices[21].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[22].position = glm::vec3(0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[23].position = glm::vec3(0.5f, -0.5f, 0.5f);
	// Left square.
	testBoxMesh.vertices[24].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[25].position = glm::vec3(-0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[26].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	//
	testBoxMesh.vertices[27].position = glm::vec3(-0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[28].position = glm::vec3(-0.5f, -0.5f, -0.5f);
	testBoxMesh.vertices[29].position = glm::vec3(-0.5f, -0.5f, 0.5f);
	// Right square.
	testBoxMesh.vertices[30].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[31].position = glm::vec3(0.5f, 0.5f, 0.5f);
	testBoxMesh.vertices[32].position = glm::vec3(0.5f, -0.5f, 0.5f);
	//
	testBoxMesh.vertices[33].position = glm::vec3(0.5f, 0.5f, -0.5f);
	testBoxMesh.vertices[34].position = glm::vec3(0.5f, -0.5f, 0.5f);
	testBoxMesh.vertices[35].position = glm::vec3(0.5f, -0.5f, -0.5f);

	// Colors.

	// Front square.
	testBoxMesh.vertices[0].color = glm::vec3(1.0f, 0.0f, 0.0f); // Red.
	testBoxMesh.vertices[1].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[2].color = glm::vec3(1.0f, 0.0f, 0.0f);
	//
	testBoxMesh.vertices[3].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[4].color = glm::vec3(1.0f, 0.0f, 0.0f);
	testBoxMesh.vertices[5].color = glm::vec3(1.0f, 0.0f, 0.0f);
	// Behind square.
	testBoxMesh.vertices[6].color = glm::vec3(0.0f, 1.0f, 0.0f); // Green.
	testBoxMesh.vertices[7].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[8].color = glm::vec3(0.0f, 1.0f, 0.0f);
	//
	testBoxMesh.vertices[9].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[10].color = glm::vec3(0.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[11].color = glm::vec3(0.0f, 1.0f, 0.0f);
	// Upper square.
	testBoxMesh.vertices[12].color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue.
	testBoxMesh.vertices[13].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[14].color = glm::vec3(0.0f, 0.0f, 1.0f);
	//
	testBoxMesh.vertices[15].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[16].color = glm::vec3(0.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[17].color = glm::vec3(0.0f, 0.0f, 1.0f);
	// Lower square.
	testBoxMesh.vertices[18].color = glm::vec3(1.0f, 1.0f, 0.0f); // Yellow.
	testBoxMesh.vertices[19].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[20].color = glm::vec3(1.0f, 1.0f, 0.0f);
	//
	testBoxMesh.vertices[21].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[22].color = glm::vec3(1.0f, 1.0f, 0.0f);
	testBoxMesh.vertices[23].color = glm::vec3(1.0f, 1.0f, 0.0f);
	// Left square.
	testBoxMesh.vertices[24].color = glm::vec3(0.0f, 1.0f, 1.0f); // Cyan.
	testBoxMesh.vertices[25].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[26].color = glm::vec3(0.0f, 1.0f, 1.0f);
	//
	testBoxMesh.vertices[27].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[28].color = glm::vec3(0.0f, 1.0f, 1.0f);
	testBoxMesh.vertices[29].color = glm::vec3(0.0f, 1.0f, 1.0f);
	// Right square.
	testBoxMesh.vertices[30].color = glm::vec3(1.0f, 0.0f, 1.0f); // Purple.
	testBoxMesh.vertices[31].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[32].color = glm::vec3(1.0f, 0.0f, 1.0f);
	//											
	testBoxMesh.vertices[33].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[34].color = glm::vec3(1.0f, 0.0f, 1.0f);
	testBoxMesh.vertices[35].color = glm::vec3(1.0f, 0.0f, 1.0f);

	// UV.

	// Front square.
	testBoxMesh.vertices[0].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[1].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[2].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	//
	testBoxMesh.vertices[3].uv = glm::vec2(0.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[4].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[5].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	// Behind square.
	testBoxMesh.vertices[6].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[7].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[8].uv = glm::vec2(1.0f, 1.0f);
	//
	testBoxMesh.vertices[9].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[10].uv = glm::vec2(1.0f, 1.0f);
	testBoxMesh.vertices[11].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	// Upper square.
	testBoxMesh.vertices[12].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[13].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[14].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	//
	testBoxMesh.vertices[15].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[16].uv = glm::vec2(0.0f, 1.0f);
	testBoxMesh.vertices[17].uv = glm::vec2(0.0f, 2.0f / 3.0f);
	// Lower square.
	testBoxMesh.vertices[18].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[19].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	testBoxMesh.vertices[20].uv = glm::vec2(1.0f / 3.0f, 1.0f);
	//
	testBoxMesh.vertices[21].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[22].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[23].uv = glm::vec2(2.0f / 3.0f, 1.0f);
	// Left square.
	testBoxMesh.vertices[24].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[25].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[26].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);
	//
	testBoxMesh.vertices[27].uv = glm::vec2(1.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[28].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[29].uv = glm::vec2(1.0f, 1.0f / 3.0f);
	// Right square.
	testBoxMesh.vertices[30].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[31].uv = glm::vec2(1.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[32].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	//
	testBoxMesh.vertices[33].uv = glm::vec2(2.0f / 3.0f, 2.0f / 3.0f);
	testBoxMesh.vertices[34].uv = glm::vec2(1.0f / 3.0f, 1.0f / 3.0f);
	testBoxMesh.vertices[35].uv = glm::vec2(2.0f / 3.0f, 1.0f / 3.0f);

	AFActor* testBoxActor0 = new AFActor();
	AFActor* testBoxActor1 = new AFActor();

	testBoxActor0->SetDisplayName("box0");
	testBoxActor1->SetDisplayName("box1");

	AFStaticMeshComponent* testBox0 = new AFStaticMeshComponent();
	testBox0->SetMesh(testBoxMesh);
	testBox0->SetTexture("content/textures/crate2.png");
	testBox0->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");

	AFStaticMeshComponent* testBox1 = new AFStaticMeshComponent();
	testBox1->SetMesh(testBoxMesh);
	testBox1->SetTexture("content/textures/crate2.png");
	testBox1->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");

	testBoxActor0->AddComponent(testBox0);
	testBoxActor1->AddComponent(testBox1);

	AddActor(testBoxActor0);
	AddActor(testBoxActor1);

	testBoxActor0->AddOffsetLocation({ 0.0f, 0.5f, 0.0f });

	testBoxActor1->AddOffsetLocation({ 3.0f, 0.5f, 0.0f });
	testBoxActor1->AddOffsetRotation({ 0.0f, 15.0f, 0.0f });
}

AFScene::AFScene()
{

}

AFScene::~AFScene()
{

}
