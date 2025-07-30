#include "AFGame.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "AFAnimGraph.h"
#include "AFAnimState.h"
#include "AFApp.h"
#include "AFContent.h"
#include "AFGraphNodeRegistry.h"
#include "AFGraphNode_Graph.h"
#include "AFGraphNode_OutputCond.h"
#include "AFGraphNode_StateCond.h"
#include "AFGraphNode_StateMachine.h"
#include "AFInput.h"
#include "AFPlayerPawn.h"
#include "AFRenderer.h"
#include "AFSkeletalMeshComponent.h"
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

	// Init camera manager.
	m_cameraManager = new AFCameraManager();
	m_cameraManager->Init(m_scene);



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

	printf("control mode: %d\n", static_cast<uint8_t>(m_currentControlMode));
}

AFGame* AFGame::GetGame()
{
	return AFApp::GetInstance().GetGame();
}

const AFScene& AFGame::GetScene() const
{
	return m_scene;
}

AFCameraManager* AFGame::GetCameraManager() const
{
	return m_cameraManager;
}

void AFGame::OnNodeCreated(const char* msg)
{
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
		graph = playerPawn->GetMeshComponent()->GetAnimState()->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}

	// The node context can be a regular graph, state machine, conditional graph or state node.
	// @todo Get rid of ifs by using some proper inheritance?
	std::shared_ptr<AFGraphNode_Graph> graphNode = std::dynamic_pointer_cast<AFGraphNode_Graph>(node);
	std::shared_ptr<AFGraphNode_StateMachine> stateMachine = std::dynamic_pointer_cast<AFGraphNode_StateMachine>(node);
	std::shared_ptr<AFGraphNode_StateCond> graphNodeCond = std::dynamic_pointer_cast<AFGraphNode_StateCond>(node);
	std::shared_ptr<AFGraphNode_State> stateNode = std::dynamic_pointer_cast<AFGraphNode_State>(node);

	if (graphNode)
	{
		graph = graphNode->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}
	if (stateMachine)
	{
		graph = stateMachine->GetStateMachine();
		graph->OnNodeCreated(msg);
		return;
	}
	if (graphNodeCond)
	{
		graph = graphNodeCond->GetGraph();
		graph->OnNodeCreated(msg);
		return;
	}
	if (stateNode)
	{
		graph = stateNode->GetGraph();
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

void AFGame::OnStateConnectionCreated(const char* msg)
{
	nlohmann::json json = nlohmann::json::parse(msg);
	const auto& elem = json[0];
	const std::string& nodeContext = elem["nodeContext"];

	// Find already existing node and use it if context matches.
	std::shared_ptr<AFGraphNode> node = AFGraphNodeRegistry::Get().GetNode(nodeContext);
	if (!node)
	{
		return;
	}

	std::shared_ptr<AFGraphNode_StateMachine> stateMachine = std::dynamic_pointer_cast<AFGraphNode_StateMachine>(node);
	if(!stateMachine)
	{
		return;
	}

	std::shared_ptr<AFStateMachine> sm = stateMachine->GetStateMachine();
	if (!sm)
	{
		return;
	}

	sm->OnConnectionCreated(msg);
}

void AFGame::OnStateConnectionRemoved(const char* msg)
{
	nlohmann::json json = nlohmann::json::parse(msg);
	const auto& elem = json[0];
	const std::string& nodeContext = elem["nodeContext"];

	// Find already existing node and use it if context matches.
	std::shared_ptr<AFGraphNode> node = AFGraphNodeRegistry::Get().GetNode(nodeContext);
	if (!node)
	{
		return;
	}

	std::shared_ptr<AFGraphNode_StateMachine> stateMachine = std::dynamic_pointer_cast<AFGraphNode_StateMachine>(node);
	if (!stateMachine)
	{
		return;
	}

	std::shared_ptr<AFStateMachine> sm = stateMachine->GetStateMachine();
	if (!sm)
	{
		return;
	}

	sm->OnConnectionRemoved(msg);
}

void AFGame::SetControlMode(EAFControlMode newControlMode)
{
	if (m_currentControlMode == newControlMode)
	{
		return;
	}

	m_currentControlMode = newControlMode;
	AFInput::UnbindAllInputs();

	switch (newControlMode)
	{
	case EAFControlMode::Editor:
	{
		SetEditorControlMode(EAFEditorControlMode::Normal);
		m_cameraManager->UpdateState();

		AFInput::BindAction("Select", [this]()
			{
				const glm::ivec2& cursorPos = AFUtility::IsMobile() ? AFInput::GetInstance().GetTouchPos(0) : AFInput::GetInstance().GetCursorPos();
				const FAFPickID& colorID = AFRenderer::ReadColorIdFromPixelCoord(cursorPos.x, cursorPos.y);
				OnSelect(colorID);
			}, EAFKeyAction::Pressed);

		AFInput::BindAxis("CameraYaw", [this](float deltaX)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddYaw(deltaX);
				}
			});
		AFInput::BindAxis("CameraPitch", [this](float deltaY)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddPitch(deltaY);
				}
			});

		AFInput::BindAxis("CameraYaw_Stroke", [this](float deltaX)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddYawStroke(deltaX);
				}
			});
		AFInput::BindAxis("CameraPitch_Stroke", [this](float deltaY)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddPitchStroke(deltaY);
				}
			});

		AFInput::BindAxis("ForwardBackward", [this](float axis)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->ForwardBackward(axis);
				}
			});
		AFInput::BindAxis("RightLeft", [this](float axis)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->RightLeft(axis);
				}
			});
		AFInput::BindAxis("UpDown", [this](float axis)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->UpDown(axis);
				}
			});

		AFInput::BindAxis("Zoom_Stroke", [this](float axis)
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->ZoomStroke(axis);
				}
			});

		AFInput::BindAction("CameraAddSpeed", [this]
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddCameraSpeedMultiplier(1.0f);
				}
			}, EAFKeyAction::Pressed);
		AFInput::BindAction("CameraLowerSpeed", [this]
			{
				if (m_currentEditorControlMode == EAFEditorControlMode::FreeView)
				{
					m_cameraManager->AddCameraSpeedMultiplier(-1.0f);
				}
			}, EAFKeyAction::Pressed);

		AFInput::BindAction("FreeViewMode", [this]
			{
				SetEditorControlMode(EAFEditorControlMode::FreeView);
			}, EAFKeyAction::Pressed);

		AFInput::BindAction("FreeViewMode", [this]
			{
				SetEditorControlMode(EAFEditorControlMode::Normal);
			}, EAFKeyAction::Released);

		AFInput::BindAction("ToggleControlMode", [this]()
			{
				SetControlMode(EAFControlMode::ActionRPG);
			}, EAFKeyAction::Pressed);

		break;
	}
	case EAFControlMode::ActionRPG:
	{
		AFApp::GetInstance().SetCursorHidden(true);
		m_cameraManager->UpdateState();

		AFInput::BindAction("ToggleControlMode", [this]()
			{
				SetControlMode(EAFControlMode::Editor);
			}, EAFKeyAction::Pressed);

		break;
	}
	default:
	{
		break;
	}
	}
}

EAFControlMode AFGame::GetControlMode() const
{
	return m_currentControlMode;
}

void AFGame::BeginPlay()
{
	m_scene.BeginPlay();

	m_cameraManager->BeginPlay();
	SetControlMode(EAFControlMode::Editor);
}

void AFGame::OnSelect(const FAFPickID& pickID)
{
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

void AFGame::SetEditorControlMode(EAFEditorControlMode newEditorControlMode)
{
	switch (newEditorControlMode)
	{
		case EAFEditorControlMode::Normal:
		{
			m_currentEditorControlMode = EAFEditorControlMode::Normal;
			AFApp::GetInstance().SetCursorHidden(false);
			break;
		}
		case EAFEditorControlMode::FreeView:
		{
			m_currentEditorControlMode = EAFEditorControlMode::FreeView;
			AFApp::GetInstance().SetCursorHidden(true);
			break;
		}
		default:
		{
			break;
		}
	}
}

AFGame::AFGame()
{

}

AFGame::~AFGame()
{

}
