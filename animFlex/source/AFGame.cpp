#include "AFGame.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AFAnimGraph.h"
#include "AFAnimState.h"
#include "AFApp.h"
#include "AFCamera.h"
#include "AFContent.h"
#include "AFGraphNodeRegistry.h"
#include "AFGraphNode_Graph.h"
#include "AFGraphNode_OutputCond.h"
#include "AFGraphNode_StateCond.h"
#include "AFGraphNode_StateMachine.h"
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

void AFGame::OnNodeCreated(const char* msg)
{
	printf("************************************\n");
	printf("received node created signal: %s\n", msg);

	std::shared_ptr<AFPlayerPawn> playerPawn = GetScene().GetSceneData().playerPawn;
	std::shared_ptr<AFAnimGraph> graph = nullptr;

	// Try to find context for this node.
	// Some of the nodes represent nested graphs.
	nlohmann::json json = nlohmann::json::parse(msg);
	const auto& elem = json[0];

	const std::string& nodeContext = elem["nodeContext"];

	// Find already existing node and use it if context matches.
	std::shared_ptr<AFGraphNode> node = AFGraphNodeRegistry::Get().GetNode(nodeContext);
	if (!node)
	{
		// Node like that doesn't exist so we assume we create a new node in the main graph.
		printf("context is main\n");
		graph = playerPawn->GetMeshComponent()->GetAnimState()->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}

	// The node context can be a regular graph, state machine or conditional graph.
	// @todo Get rid of ifs by using some proper inheritance?
	std::shared_ptr<AFGraphNode_Graph> graphNode = std::dynamic_pointer_cast<AFGraphNode_Graph>(node);
	std::shared_ptr<AFGraphNode_StateMachine> stateMachine = std::dynamic_pointer_cast<AFGraphNode_StateMachine>(node);
	std::shared_ptr<AFGraphNode_StateCond> graphNodeCond = std::dynamic_pointer_cast<AFGraphNode_StateCond>(node);

	if (graphNode)
	{
		printf("context is regular graph\n");
		graph = graphNode->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}
	if (stateMachine)
	{
		printf("context is state machine\n");
		graph = stateMachine->GetStateMachine();
		graph->OnNodeCreated(msg);
		return;
	}
	if (graphNodeCond)
	{
		printf("context is conditional graph\n");
		graph = graphNodeCond->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}
}

void AFGame::OnNodeUpdated(const char* msg)
{
	AFAnimGraph::OnNodeUpdated(msg);
}

void AFGame::OnNodeRemoved(const char* msg)
{
	AFAnimGraph::OnNodeRemoved(msg);
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
