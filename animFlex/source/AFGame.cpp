#include "AFGame.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AFAnimGraph.h"
#include "AFAnimState.h"
#include "AFApp.h"
#include "AFCamera.h"
#include "AFContent.h"
#include "AFInput.h"
#include "AFPlayerPawn.h"
#include "AFSerializer.h"
#include "AFSkeletalMeshComponent.h"
#include "AFTimerManager.h"
#include "AFUIComponent.h"
#include "AFUtility.h"
#include "IAFPickerInterface.h"
#include "AFUI.h"

#ifdef __EMSCRIPTEN__
#include <sys/stat.h>
#include <sys/types.h>
#endif

bool AFGame::Init()
{
	// Init scene and default objects.
	if (!m_scene.Init())
	{
		printf("%s\n", "Scene Init() failed.");
		return false;
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

void AFGame::OnGraphUpdate(const char* graphState)
{
	std::shared_ptr<AFPlayerPawn> playerPawn = GetScene().GetSceneData().playerPawn;
	std::shared_ptr<AFAnimGraph> graph = playerPawn->GetMeshComponent()->GetAnimState()->GetGraph();
	if (graph)
	{
		graph->Compile(graphState);
	}
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
