#include "AFScene.h"

#include "AFAnimState.h"
#include "AFApp.h"
#include "AFUI.h"
#include "AFBackgroundComponent.h"
#include "AFBoxComponent.h"
#include "AFContent.h"
#include "AFGridComponent.h"
#include "AFMath.h"
#include "AFOrientationBoxUIComponent.h"
#include "AFOrientationGizmoUIComponent.h"
#include "AFOrientationGlyph.h"
#include "AFOrientGizmo.h"
#include "AFPlayerPawn.h"
#include "AFSkeletalMeshComponent.h"
#include "AFStaticMeshComponent.h"
#include "AFTextComponent.h"
#include "AFUIRenderComponent.h"
#include "AFUtility.h"
#include "AFVertexBuffer.h"
#include "AFCamera.h"

std::shared_ptr<AFActor> AFScene::FindActor(const std::string& actorName)
{
	auto it = std::find_if(m_sceneData.sceneActors.begin(), m_sceneData.sceneActors.end(),
		[actorName](std::shared_ptr<AFActor> actor)
		{
			return actor && actor->GetDisplayName() == actorName;
		});

	if(it != m_sceneData.sceneActors.end())
	{
		return *it;
	}

	return nullptr;
}

void AFScene::AddActor(std::shared_ptr<AFActor> newActor)
{
	if(!newActor)
	{
		return;
	}

	m_sceneData.sceneActors.push_back(newActor);
}

void AFScene::AddUI(std::shared_ptr<AFUI> newUI)
{
	if (!newUI)
	{
		return;
	}

	m_sceneData.uis.push_back(newUI);
}

void AFScene::SetActiveCamera(std::shared_ptr<AFCamera> newActiveCamera)
{
	m_sceneData.activeCamera = newActiveCamera;
}

std::shared_ptr<AFCamera> AFScene::GetActiveCamera() const
{
	return m_sceneData.activeCamera;
}

void AFScene::SetPlayerPawn(std::shared_ptr<AFPlayerPawn> newPlayerPawn)
{
	m_sceneData.playerPawn = newPlayerPawn;
}

std::shared_ptr<AFPlayerPawn> AFScene::GetPlayerPawn() const
{
	return m_sceneData.playerPawn;
}

const FAFSceneData& AFScene::GetSceneData() const
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
	// Background.
	std::shared_ptr<AFActor> backgroundActor = CreateObject<AFActor>();
	backgroundActor->SetDisplayName("background actor");
	std::shared_ptr<AFBackgroundComponent> backgroundMeshComponent = CreateObject<AFBackgroundComponent>();
	backgroundMeshComponent->SetDisplayName("background mesh component");
	backgroundMeshComponent->m_backgroundShader = AFContent::Get().FindAsset<AFShader>("shader_background"); // Assign just the shader. Background is fully procedural.
	backgroundActor->AddComponent(backgroundMeshComponent);
	AddActor(backgroundActor);

	// Grid.
	std::shared_ptr<AFActor> gridActor = CreateObject<AFActor>();
	gridActor->SetDisplayName("grid actor");
	std::shared_ptr<AFGridComponent> gridMeshComponent = CreateObject<AFGridComponent>();
	gridMeshComponent->SetDisplayName("grid mesh component");
	std::shared_ptr<FAFMesh> gridMesh = AFContent::Get().FindAsset<FAFMesh>("sm_plane");
	gridMesh->subMeshes[0].shader = AFContent::Get().FindAsset<AFShader>("shader_grid");
	gridMeshComponent->SetMesh(gridMesh);
	gridActor->AddComponent(gridMeshComponent);
	AddActor(gridActor);

	// Test boxes.
	/*std::shared_ptr<AFActor> testBoxActor0 = CreateObject<AFActor>();
	std::shared_ptr<AFActor> testBoxActor1 = CreateObject<AFActor>();
	std::shared_ptr<AFActor> testBoxActor2 = CreateObject<AFActor>();
	testBoxActor0->SetDisplayName("testbox actor0");
	testBoxActor1->SetDisplayName("testbox actor1");
	testBoxActor2->SetDisplayName("testbox actor2");
	std::shared_ptr<AFBoxComponent> testBoxMeshComponent0 = CreateObject<AFBoxComponent>();
	std::shared_ptr<AFBoxComponent> testBoxMeshComponent1 = CreateObject<AFBoxComponent>();
	std::shared_ptr<AFBoxComponent> testBoxMeshComponent2 = CreateObject<AFBoxComponent>();
	testBoxMeshComponent0->SetDisplayName("testbox mesh component0");
	testBoxMeshComponent1->SetDisplayName("testbox mesh component1");
	testBoxMeshComponent2->SetDisplayName("testbox mesh component2");
	std::shared_ptr<FAFMesh> testBoxMesh = AFContent::Get().FindAsset<FAFMesh>("sm_box");
	testBoxMesh->subMeshes[0].texture = AFContent::Get().FindAsset<AFTexture>("t_box");
	testBoxMesh->subMeshes[0].shader = AFContent::Get().FindAsset<AFShader>("shader_basic");
	testBoxMeshComponent0->SetMesh(testBoxMesh);
	testBoxMeshComponent1->SetMesh(testBoxMesh);
	testBoxMeshComponent2->SetMesh(testBoxMesh);
	testBoxActor0->AddComponent(testBoxMeshComponent0);
	testBoxActor1->AddComponent(testBoxMeshComponent1);
	testBoxActor2->AddComponent(testBoxMeshComponent2);
	AddActor(testBoxActor0);
	testBoxActor0->AddOffsetLocation({ 0.0f, 50.0f, 0.0f });
	AddActor(testBoxActor1);
	testBoxActor1->AddOffsetLocation({ 300.0f, 50.0f, 0.0f });
	testBoxActor1->AddOffsetRotation({ 0.0f, 15.0f, 0.0f });
	AddActor(testBoxActor2);
	testBoxActor2->AddOffsetLocation({ -350.0f, 50.0f, 50.0f });*/

	// Create default camera.
	std::shared_ptr<AFCamera> initCamera = CreateObject<AFCamera>();
	initCamera->SetLocation({ 0.0f, 100.0f, 220.0f });
	SetActiveCamera(initCamera);
	AddActor(initCamera);

	// Create default player.
	std::shared_ptr<AFPlayerPawn> playerPawn = AFScene::CreateObject<AFPlayerPawn>();
	SetPlayerPawn(playerPawn);
	AddActor(playerPawn);

	// Mannequin.
	std::shared_ptr<AFPlayerPawn> mannequinActor = m_sceneData.playerPawn;
	mannequinActor->SetDisplayName("mannequin actor");
	mannequinActor->GetMeshComponent()->SetDisplayName("mannequin mesh component");
	std::shared_ptr<FAFMesh> mannequinMesh = AFContent::Get().FindAsset<FAFMesh>("sk_mannequin");
	mannequinActor->GetMeshComponent()->SetMesh(mannequinMesh);
	std::shared_ptr<AFAnimGraph> playerAnimGraph = CreateObject<AFAnimGraph>();
	mannequinActor->GetMeshComponent()->GetAnimState()->SetGraph(playerAnimGraph);
}

void AFScene::CreateDefaultUIs()
{
	// Orientation gizmo.
	std::shared_ptr<AFOrientGizmo> orientationGizmo = CreateObject<AFOrientGizmo>();
	orientationGizmo->SetDisplayName("orientation gizmo ui");
	std::shared_ptr<AFOrientationGizmoUIComponent> orientationGizmoComponent = CreateObject<AFOrientationGizmoUIComponent>();
	orientationGizmoComponent->SetDisplayName("orientation gizmo component");
	std::shared_ptr<FAFMesh> orientationGizmoMesh = AFContent::Get().FindAsset<FAFMesh>("sm_gizmo");
	orientationGizmoMesh->subMeshes[0].shader = AFContent::Get().FindAsset<AFShader>("shader_gizmo");
	orientationGizmoComponent->SetMesh(orientationGizmoMesh);
	orientationGizmo->AddComponent(orientationGizmoComponent);
	orientationGizmoComponent->SetLocation(glm::vec2(-0.85f, -0.80f));
	orientationGizmoComponent->SetScale({ 0.15f,  0.15f });

	// Orientation gizmo letters.
	const glm::vec2 textGlyphLocation = glm::vec2(-0.85f, -0.80f);
	const float textGlyphScale = 0.5f;
	std::shared_ptr<AFShader> glyphShader = AFContent::Get().FindAsset<AFShader>("shader_glyph");
	std::shared_ptr<AFOrientationGlyph> xGlyph = CreateObject<AFOrientationGlyph>(); // X-glyph.
	xGlyph->SetDisplayName("orientation x glyph");
	xGlyph->SetText("x");
	orientationGizmo->AddComponent(xGlyph);
	xGlyph->SetLocation(textGlyphLocation);
	xGlyph->SetScale(glm::vec2(textGlyphScale));
	std::shared_ptr<AFOrientationGlyph> yGlyph = CreateObject<AFOrientationGlyph>(); // Y-glyph.
	yGlyph->SetDisplayName("orientation y glyph");
	yGlyph->SetText("y");
	orientationGizmo->AddComponent(yGlyph);
	yGlyph->SetLocation(textGlyphLocation);
	yGlyph->SetScale(glm::vec2(textGlyphScale));
	std::shared_ptr<AFOrientationGlyph> zGlyph = CreateObject<AFOrientationGlyph>(); // Z-glyph.
	zGlyph->SetDisplayName("orientation z glyph");
	zGlyph->SetText("z");
	orientationGizmo->AddComponent(zGlyph);
	zGlyph->SetLocation(textGlyphLocation);
	zGlyph->SetScale(glm::vec2(textGlyphScale));
	AddUI(orientationGizmo);

	// Create orientation box.
	std::shared_ptr<AFUI> orientationBoxUI = CreateObject<AFUI>();
	orientationBoxUI->SetDisplayName("orientation box ui");
	std::shared_ptr<AFOrientationBoxUIComponent> orientationBoxComp = CreateObject<AFOrientationBoxUIComponent>();
	orientationBoxComp->SetDisplayName("orientation box component");
	std::shared_ptr<FAFMesh> orientationBoxMesh = AFContent::Get().FindAsset<FAFMesh>("sm_box");
	orientationBoxMesh->subMeshes[0].texture = AFContent::Get().FindAsset<AFTexture>("t_box");
	orientationBoxComp->SetMesh(orientationBoxMesh);
	orientationBoxUI->AddComponent(orientationBoxComp);
	orientationBoxComp->SetLocation(glm::vec2(0.85f, 0.80f));
	orientationBoxComp->SetScale(glm::vec2(0.001f, 0.001f));
	AddUI(orientationBoxUI); 
}

AFScene::AFScene()
{

}

AFScene::~AFScene()
{

}
