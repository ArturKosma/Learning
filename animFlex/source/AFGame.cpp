#include "AFGame.h"

#include "AFApp.h"
#include "AFCamera.h"
#include "AFInput.h"
#include "AFTimerManager.h"
#include "AFUIComponent.h"
#include "AFUtility.h"
#include "IAFPickerInterface.h"
#include "AFUI.h"

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

	for (AFUI* ui : m_scene.GetSceneData().uis)
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

	for (const AFActor* const sceneActor : m_scene.GetSceneData().sceneActors)
	{
		// Per component select.
		for (AFComponent* component : sceneActor->GetComponents())
		{
			if(component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			IAFPickerInterface* pickerInterface = dynamic_cast<IAFPickerInterface*>(component);

			if (!pickerInterface)
			{
				continue;
			}

			pickerInterface->OnClickPressed(pickID.elementId);

			return;
		}
	}

	for (const AFUI* const ui : m_scene.GetSceneData().uis)
	{
		// Per component select.
		for (AFComponent* component : ui->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			IAFPickerInterface* pickerInterface = dynamic_cast<IAFPickerInterface*>(component);

			if (!pickerInterface)
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

	AFObject* newHover = nullptr;

	for (const AFActor* const sceneActor : m_scene.GetSceneData().sceneActors)
	{
		// Per component select.
		for (AFComponent* component : sceneActor->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			IAFPickerInterface* pickerInterface = dynamic_cast<IAFPickerInterface*>(component);

			if (!pickerInterface)
			{
				continue;
			}

			newHover = component;
			break;
		}
	}

	for (const AFUI* const ui : m_scene.GetSceneData().uis)
	{
		// Per component select.
		for (AFComponent* component : ui->GetComponents())
		{
			if (component->GetUniqueID() != pickID.objectId)
			{
				continue;
			}

			IAFPickerInterface* pickerInterface = dynamic_cast<IAFPickerInterface*>(component);

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
		IAFPickerInterface* oldHoverInterface = dynamic_cast<IAFPickerInterface*>(m_currentHover);
		if(oldHoverInterface)
		{
			oldHoverInterface->OnHoverEnd(m_currentHoverElement);
		}

		IAFPickerInterface* newHoverInterface = dynamic_cast<IAFPickerInterface*>(newHover);
		if(newHoverInterface)
		{
			newHoverInterface->OnHoverBegin(pickID.elementId);
		}

		m_currentHover = newHover;
		m_currentHoverElement = pickID.elementId;
	}
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}
