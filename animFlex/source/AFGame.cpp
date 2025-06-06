#include "AFGame.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AFApp.h"
#include "AFCamera.h"
#include "AFContent.h"
#include "AFInput.h"
#include "AFSkeletalMeshComponent.h"
#include "AFTimerManager.h"
#include "AFUIComponent.h"
#include "AFUtility.h"
#include "IAFPickerInterface.h"
#include "AFUI.h"

bool AFGame::Init()
{
	// Init scene and default objects.
	if (!m_scene.Init())
	{
		printf("%s\n", "Scene Init() failed.");
		return false;
	}

	// Create default camera.
	std::shared_ptr<AFCamera> initCamera = AFScene::CreateObject<AFCamera>();
	m_scene.AddActor(initCamera);
	initCamera->SetLocation({ 0.0f, 100.0f, 220.0f });
	m_scene.SetActiveCamera(initCamera);

	// Start playing single anim on the mannequin.
	std::shared_ptr<AFActor> mannequinActor = m_scene.FindActor("mannequin actor");
	if (mannequinActor)
	{
		std::shared_ptr<AFSkeletalMeshComponent> mannequinMesh = std::dynamic_pointer_cast<AFSkeletalMeshComponent>(mannequinActor->GetComponentByName("mannequin mesh component"));
		if (mannequinMesh)
		{
			std::shared_ptr<AFAnimationClip> idle = AFContent::Get().FindAsset<AFAnimationClip>("anim_idle");
			std::shared_ptr<AFAnimationClip> runF = AFContent::Get().FindAsset<AFAnimationClip>("anim_runF");
			std::shared_ptr<AFAnimationClip> runF2idle = AFContent::Get().FindAsset<AFAnimationClip>("anim_runF2idle");
			std::shared_ptr<AFAnimationClip> idle2runF = AFContent::Get().FindAsset<AFAnimationClip>("anim_idle2runF");

			std::shared_ptr<AFAnimationClip> clip = idle2runF;
			if (clip)
			{
				mannequinMesh->SetAnimation(clip);
				mannequinMesh->AnimationPlay();
			}
		}
	}

	return true;
}

void AFGame::Tick(float deltaTime)
{
	for(std::shared_ptr<AFActor> actor : m_scene.GetSceneData().sceneActors)
	{
		actor->Tick(deltaTime);
	}

	for (std::shared_ptr<AFUI> ui : m_scene.GetSceneData().uis)
	{
		ui->Tick(deltaTime);
	}
}

AFGame* AFGame::GetGame()
{
	return AFApp::GetInstance().GetGame();
}

const AFScene& AFGame::GetScene()
{
	return m_scene;
}

void AFGame::OnSelect(const FAFPickID& pickID)
{
	if(AFInput::GetFreeViewMode())
	{
		return;
	}

	for (std::shared_ptr<AFActor> sceneActor : m_scene.GetSceneData().sceneActors)
	{
		// Per component select.
		for (std::shared_ptr<AFComponent> component : sceneActor->GetComponents())
		{
			if(component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);

			if (!pickerInterface)
			{
				continue;
			}

			const std::vector<uint8_t> disabledElements = pickerInterface->GetDisabledElements();
			const bool containsDisabled = std::find(disabledElements.begin(), disabledElements.end(), pickID.elementId) != disabledElements.end();
			if (containsDisabled)
			{
				continue;
			}

			pickerInterface->OnClickPressed(pickID.elementId);

			return;
		}
	}

	for (std::shared_ptr<AFUI> ui : m_scene.GetSceneData().uis)
	{
		// Per component select.
		for (std::shared_ptr<AFComponent> component : ui->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);

			if (!pickerInterface)
			{
				continue;
			}

			const std::vector<uint8_t> disabledElements = pickerInterface->GetDisabledElements();
			const bool containsDisabled = std::find(disabledElements.begin(), disabledElements.end(), pickID.elementId) != disabledElements.end();
			if (containsDisabled)
			{
				continue;
			}

			pickerInterface->OnClickPressed(pickID.elementId);

			return;
		}
	}
}

void AFGame::OnHover(const FAFPickID& pickID)
{
	if (AFInput::GetFreeViewMode())
	{
		return;
	}

	std::shared_ptr<AFObject> newHover = nullptr;
	uint8_t newHoverElement = -1;

	for (std::shared_ptr<AFActor> sceneActor : m_scene.GetSceneData().sceneActors)
	{
		// Per component select.
		for (std::shared_ptr<AFComponent> component : sceneActor->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);

			if (!pickerInterface)
			{
				continue;
			}

			newHover = component;
			break;
		}
	}

	for (std::shared_ptr<AFUI> ui : m_scene.GetSceneData().uis)
	{
		// Per component select.
		for (std::shared_ptr<AFComponent> component : ui->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);

			if (!pickerInterface)
			{
				continue;
			}

			newHover = component;
			break;
		}
	}

	if((newHover != m_currentHover) || (pickID.elementId != m_currentHoverElement))
	{
		std::shared_ptr<IAFPickerInterface> oldHoverInterface = std::dynamic_pointer_cast<IAFPickerInterface>(m_currentHover);
		if(oldHoverInterface)
		{
			oldHoverInterface->OnHoverEnd(m_currentHoverElement);
		}

		std::shared_ptr<IAFPickerInterface> newHoverInterface = std::dynamic_pointer_cast<IAFPickerInterface>(newHover);
		if(newHoverInterface)
		{
			const std::vector<uint8_t> disabledElements = newHoverInterface->GetDisabledElements();
			const bool containsDisabled = std::find(disabledElements.begin(), disabledElements.end(), pickID.elementId) != disabledElements.end();
			if (containsDisabled)
			{
				newHover = nullptr;
			}
			else
			{
				newHoverInterface->OnHoverBegin(pickID.elementId);
				newHoverElement = pickID.elementId;
			}
		}

		m_currentHover = newHover;
		m_currentHoverElement = newHoverElement;
	}
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}
