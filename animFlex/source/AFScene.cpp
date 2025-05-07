#include "AFScene.h"

#include "AFUI.h"
#include "AFBackgroundComponent.h"
#include "AFBoxComponent.h"
#include "AFGLTFLoader.h"
#include "AFGridComponent.h"
#include "AFOrientationBoxUIComponent.h"
#include "AFOrientationGizmoUIComponent.h"
#include "AFOrientationGlyph.h"
#include "AFStaticMeshComponent.h"
#include "AFTextComponent.h"
#include "AFUIRenderComponent.h"
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

void AFScene::AddUI(AFUI* newUI)
{
	if (!newUI)
	{
		return;
	}

	// Upon adding new UI to the scene, load all of its renderable assets.
	for (AFComponent* comp : newUI->GetComponents())
	{
		AFUIRenderComponent* const uiRenderComp = dynamic_cast<AFUIRenderComponent*>(comp);
		if (!uiRenderComp)
		{
			continue;
		}
		uiRenderComp->Load();
	}

	m_sceneData.uis.push_back(newUI);
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
	// Reserve some space for the scene & ui.
	m_sceneData.sceneActors.reserve(20);
	m_sceneData.uis.reserve(20);

	CreateDefaultSceneActors();
	CreateDefaultUIs();

	return true;
}

void AFScene::CreateDefaultSceneActors()
{
	// Create background.
	AFActor* background = CreateObject<AFActor>();
	AFBackgroundComponent* backgroundMesh = CreateObject<AFBackgroundComponent>();
	background->AddComponent(backgroundMesh);
	background->SetDisplayName("background");
	AddActor(background);

	// Create a grid.
	AFActor* gridActor = CreateObject<AFActor>();
	gridActor->SetDisplayName("grid");
	AFGridComponent* gridComponent = CreateObject<AFGridComponent>();
	gridActor->AddComponent(gridComponent);
	AddActor(gridActor);

	// Create test boxes.
	AFActor* testBoxActor0 = CreateObject<AFActor>();
	AFActor* testBoxActor1 = CreateObject<AFActor>();
	AFActor* testBoxActor2 = CreateObject<AFActor>();
	testBoxActor0->SetDisplayName("box0");
	testBoxActor1->SetDisplayName("box1");
	testBoxActor2->SetDisplayName("box2");
	AFBoxComponent* testBox0 = CreateObject<AFBoxComponent>();
	testBox0->SetTexture("content/textures/crate2.png");
	testBox0->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	AFBoxComponent* testBox1 = CreateObject<AFBoxComponent>();
	testBox1->SetTexture("content/textures/crate2.png");
	testBox1->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	AFBoxComponent* testBox2 = CreateObject<AFBoxComponent>();
	testBox2->SetTexture("content/textures/crate2.png");
	testBox2->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	testBoxActor0->AddComponent(testBox0);
	testBoxActor1->AddComponent(testBox1);
	testBoxActor2->AddComponent(testBox2);
	AddActor(testBoxActor0);
	AddActor(testBoxActor1);
	AddActor(testBoxActor2);
	testBoxActor0->AddOffsetLocation({ 0.0f, 50.0f, 0.0f });
	testBoxActor1->AddOffsetLocation({ 300.0f, 50.0f, 0.0f });
	testBoxActor1->AddOffsetRotation({ 0.0f, 15.0f, 0.0f });
	testBoxActor2->AddOffsetLocation({ -350.0f, 50.0f, 50.0f });

	// TinyGLTF.
	AFActor* tinyglTFActor = CreateObject<AFActor>();
	tinyglTFActor->SetDisplayName("tinyglTF");
	AFStaticMeshComponent* tinyglTFMesh = CreateObject<AFStaticMeshComponent>();
	tinyglTFMesh->SetMesh(AFGLTFLoader::Load("content/models/woman.gltf"));
	tinyglTFMesh->SetTexture("content/textures/crate2.png");
	tinyglTFMesh->SetShaders("content/shaders/basic.vert", "content/shaders/basic.frag");
	tinyglTFActor->AddComponent(tinyglTFMesh);
	tinyglTFMesh->SetLocalScale(glm::vec3(100.0f));
	AddActor(tinyglTFActor);
}

void AFScene::CreateDefaultUIs()
{
	// Create orientation gizmo.
	AFUI* orientationGizmo = CreateObject<AFUI>();
	orientationGizmo->SetDisplayName("Orientation Gizmo UI");
	AFOrientationGizmoUIComponent* orientationGizmoComponent = CreateObject<AFOrientationGizmoUIComponent>();
	orientationGizmo->AddComponent(orientationGizmoComponent);
	orientationGizmoComponent->SetLocation(glm::vec2(-0.85f, -0.80f));
	orientationGizmoComponent->SetScale(glm::vec2(0.15f, 0.15f));

	const float textGlyphScale = 0.68f;

	// Create xyz letters around the orientation gizmo.
	AFOrientationGlyph* xGlyph = CreateObject<AFOrientationGlyph>(); // X-glyph.
	xGlyph->SetText("x");
	orientationGizmo->AddComponent(xGlyph);
	xGlyph->SetLocation(glm::vec2(-0.85f, -0.80f));
	xGlyph->SetScale(glm::vec2(textGlyphScale));
	xGlyph->SetShaders("content/shaders/glyph.vert", "content/shaders/glyph.frag");

	AFOrientationGlyph* yGlyph = CreateObject<AFOrientationGlyph>(); // Y-glyph.
	yGlyph->SetText("y");
	orientationGizmo->AddComponent(yGlyph);
	yGlyph->SetLocation(glm::vec2(-0.85f, -0.80f));
	yGlyph->SetScale(glm::vec2(textGlyphScale));
	yGlyph->SetShaders("content/shaders/glyph.vert", "content/shaders/glyph.frag");

	AFOrientationGlyph* zGlyph = CreateObject<AFOrientationGlyph>(); // Z-glyph.
	zGlyph->SetText("z");
	orientationGizmo->AddComponent(zGlyph);
	zGlyph->SetLocation(glm::vec2(-0.85f, -0.80f));
	zGlyph->SetScale(glm::vec2(textGlyphScale));
	zGlyph->SetShaders("content/shaders/glyph.vert", "content/shaders/glyph.frag");

	AddUI(orientationGizmo);

	// Create orientation box.
	AFUI* orientationBox = CreateObject<AFUI>();
	orientationBox->SetDisplayName("Orientation Box UI");
	AFOrientationBoxUIComponent* orientationBoxComp = CreateObject<AFOrientationBoxUIComponent>();
	orientationBox->AddComponent(orientationBoxComp);
	orientationBoxComp->SetLocation(glm::vec2(0.85f, 0.80f));
	orientationBoxComp->SetScale(glm::vec2(0.05f, 0.05f));
	AddUI(orientationBox);
}

AFScene::AFScene()
{

}

AFScene::~AFScene()
{

}
