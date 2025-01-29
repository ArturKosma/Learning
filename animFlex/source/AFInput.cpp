#include "AFInput.h"
#include "AFConfig.h"
#include "AFUtility.h"

#include <algorithm>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

void AFInput::OnKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	// Update the state of all pressed/released keys.
	m_keystate.insert_or_assign(key, static_cast<float>(std::clamp(action, 0, 1)));

	// See if the key is configured.
	const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();
	for (const auto& [configAction, configKey] : configMappings.actionMappings)
	{
		if (configKey == key)
		{
			const auto& foundBound = m_boundActionMappings.find(configAction);
			if (foundBound != m_boundActionMappings.end())
			{
				if (action == GLFW_PRESS)
				{
					for(const std::function<void()>& pressedFunctor : foundBound->second.onPressedFunctors)
					{
						pressedFunctor();
					}
				}
				else if (action == GLFW_RELEASE)
				{
					for (const std::function<void()>& releasedFunctor : foundBound->second.onReleasedFunctors)
					{
						releasedFunctor();
					}
				}
			}
		}
	}
}

void AFInput::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (!window)
	{
		return;
	}

	const ImGuiIO& io = ImGui::GetIO();
	if (io.WantCaptureMouse)
	{
		//ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		return;
	}

	// Update the state of all pressed/released keys.
	m_keystate.insert_or_assign(button, static_cast<float>(action));

	// See if the button is configured.
	const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();
	for (const auto& [configAction, configKey] : configMappings.actionMappings)
	{
		if (configKey == button)
		{
			const auto& foundBound = m_boundActionMappings.find(configAction);
			if (foundBound != m_boundActionMappings.end())
			{
				if (action == GLFW_PRESS)
				{
					for (const std::function<void()>& pressedFunctor : foundBound->second.onPressedFunctors)
					{
						pressedFunctor();
					}
				}
				else if (action == GLFW_RELEASE)
				{
					for (const std::function<void()>& releasedFunctor : foundBound->second.onReleasedFunctors)
					{
						releasedFunctor();
					}
				}
			}
		}
	}
}

void AFInput::OnCursorPosCallback(GLFWwindow* window, double posX, double posY)
{
	if(!window)
	{
		return;
	}

	const float deltaX = static_cast<float>(static_cast<int>(posX) - m_cursorXPos);
	const float deltaY = static_cast<float>(static_cast<int>(posY) - m_cursorYPos);

	if(deltaX > 0.0f)
	{
		m_keystate.insert_or_assign(1001, deltaX);
		m_keystate.insert_or_assign(1002, 0.0f);
	}
	else if(deltaX < 0.0f)
	{
		m_keystate.insert_or_assign(1001, 0.0f);
		m_keystate.insert_or_assign(1002, abs(deltaX));
	}

	if (deltaY > 0.0f)
	{
		m_keystate.insert_or_assign(1003, deltaY);
		m_keystate.insert_or_assign(1004, 0.0f);
	}
	else if (deltaY < 0.0f)
	{
		m_keystate.insert_or_assign(1003, 0.0f);
		m_keystate.insert_or_assign(1004, abs(deltaY));
	}

	m_cursorXPos = static_cast<int>(posX);
	m_cursorYPos = static_cast<int>(posY);
}

void AFInput::Tick()
{
	// Go through all bound axes and call their functors.
	for(const auto& [axisName, boundAxis] : m_boundAxisMappings)
	{
		auto foundKeystate1 = m_keystate.find(boundAxis.keyToValue1.first);
		const float state1 = foundKeystate1->second * boundAxis.keyToValue1.second;

		auto foundKeystate2 = m_keystate.find(boundAxis.keyToValue2.first);
		const float state2 = foundKeystate2->second * boundAxis.keyToValue2.second;

		const float value = state1 + state2;

		for (const std::function<void(float)>& axisFunctor : boundAxis.axisFunctors)
		{
			// Call the bound axis functor with proper value based on keymap.
			axisFunctor(value);
		}
	}

	// Clear state of inputs that are faked.

	m_keystate.insert_or_assign(1001, 0.0f); // Mouse turn right.
	m_keystate.insert_or_assign(1002, 0.0f); // Mouse turn left.
	m_keystate.insert_or_assign(1003, 0.0f); // Mouse tilt up.
	m_keystate.insert_or_assign(1004, 0.0f); // Mouse tilt down.
}

AFInput& AFInput::GetInstance()
{
	static AFInput input;
	return input;
}

void AFInput::BindAction(const std::string& actionName, const std::function<void()>& actionFunction, EAFKeyAction keyAction)
{
	// Allow binding of existing actions only.
	const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();

	auto foundAxis = configMappings.actionMappings.find(actionName);
	if (foundAxis == configMappings.actionMappings.end())
	{
		return;
	}

	FAFBoundAction actionWrapper;

	auto foundMapping = m_boundActionMappings.find(actionName);
	if(foundMapping != m_boundActionMappings.end())
	{
		actionWrapper = foundMapping->second; // Modify existing mapping.
	}

	if (keyAction == EAFKeyAction::Pressed)
	{
		actionWrapper.onPressedFunctors.push_back(actionFunction);
	}
	else if (keyAction == EAFKeyAction::Released)
	{
		actionWrapper.onReleasedFunctors.push_back(actionFunction);
	}

	m_boundActionMappings.insert_or_assign(actionName, actionWrapper);
}

void AFInput::BindAxis(const std::string& axis, const std::function<void(float)>& fun)
{
	// Allow binding of existing axes only.
	const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();

	auto foundAxis = configMappings.axisMappings.find(axis);
	if(foundAxis == configMappings.axisMappings.end())
	{
		return;
	}
	
	FAFBoundAxis boundAxis;
	boundAxis.axisFunctors.push_back(fun);
	boundAxis.keyToValue1 = foundAxis->second.keyToValue1;
	boundAxis.keyToValue2 = foundAxis->second.keyToValue2;

	m_boundAxisMappings.insert_or_assign(axis, boundAxis);
}

bool AFInput::GetFreeViewMode() const
{
	return m_freeView;
}

void AFInput::Init(GLFWwindow* window)
{
	GetInstance().m_window = window;

	// Register all configurable keys into keystate map.
	for(const auto& [action, key] : AFUtility::GetConfigurableKeys())
	{
		GetInstance().m_keystate.insert_or_assign(key, 0.0f);
	}

	GetInstance().BindAction("FreeViewMode", [] {GetInstance().Input_FreeViewMode_Pressed(); }, EAFKeyAction::Pressed);
	GetInstance().BindAction("FreeViewMode", [] {GetInstance().Input_FreeViewMode_Released(); }, EAFKeyAction::Released);
}

AFInput::AFInput()
{
	
}

AFInput::~AFInput()
{

}

void AFInput::OnScrollCallback(GLFWwindow* window, double xscroll, double yscroll)
{
	if (!window)
	{
		return;
	}
}

void AFInput::Input_FreeViewMode_Pressed()
{
	m_freeView = true;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void AFInput::Input_FreeViewMode_Released()
{
	m_freeView = false;

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
