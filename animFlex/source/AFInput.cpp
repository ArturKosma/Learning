#include "AFInput.h"

#include <cstdio>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "AFConfig.h"
#include "AFUtility.h"

void AFInput::OnKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	// Update the state of all pressed/released keys.
	m_keystate.insert_or_assign(key, action);

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
					if (foundBound->second.onPressed)
					{
						foundBound->second.onPressed();
					}
				}
				else if (action == GLFW_RELEASE)
				{
					if (foundBound->second.onReleased)
					{
						foundBound->second.onReleased();
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
	m_keystate.insert_or_assign(button, action);

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
					foundBound->second.onPressed();
				}
				else if (action == GLFW_RELEASE)
				{
					foundBound->second.onReleased();
				}
			}
		}
	}
}

void AFInput::OnCursorPosCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	/*if (!m_mouseLock)
	{
		ImGui_ImplGlfw_CursorPosCallback(window, xoffset, yoffset);
	}*/

	m_cursorXPos = xoffset;
	m_cursorYPos = yoffset;
}

void AFInput::Tick()
{
	for(const auto& [axisName, boundAxis] : m_boundAxisMappings)
	{
		if(!boundAxis.fun)
		{
			continue;
		}

		// Find the state of key1 in the axis binding
		auto foundKeystate1 = m_keystate.find(boundAxis.keyToValue1.first);
		if(foundKeystate1 == m_keystate.end())
		{
			continue;
		}
		const bool isPressed1 = foundKeystate1->second;

		auto foundKeystate2 = m_keystate.find(boundAxis.keyToValue2.first);
		if (foundKeystate2 == m_keystate.end())
		{
			continue;
		}
		const bool isPressed2 = foundKeystate2->second;

		// If both keys or none of them is pressed, send 0.0f.
		if((isPressed1 && isPressed2) || (!isPressed1 && !isPressed2))
		{
			boundAxis.fun(0.0f);
			continue;
		}

		const float value = isPressed1 ? boundAxis.keyToValue1.second : (isPressed2 ? boundAxis.keyToValue2.second : 0.0f);

		// Call the bound axis with proper value based on keymap.
		boundAxis.fun(value);
	}
}

AFInput& AFInput::GetInstance()
{
	static AFInput input;
	return input;
}

void AFInput::BindAction(const std::string& actionName, const std::function<void()>& actionFunction, EAFKeyAction keyAction)
{
	auto foundMapping = m_boundActionMappings.find(actionName);

	// Modify existing mapping.
	if(foundMapping != m_boundActionMappings.end())
	{
		FAFBoundAction foundActionFuctions = foundMapping->second;
		if(keyAction == EAFKeyAction::Pressed)
		{
			foundActionFuctions.onPressed = actionFunction;
		}
		else if(keyAction == EAFKeyAction::Released)
		{
			foundActionFuctions.onReleased = actionFunction;
		}
		m_boundActionMappings.insert_or_assign(actionName, foundActionFuctions);
	}
	// Add completely new mapping.
	else
	{
		FAFBoundAction newActionFuctions;
		if (keyAction == EAFKeyAction::Pressed)
		{
			newActionFuctions.onPressed = actionFunction;
		}
		else if (keyAction == EAFKeyAction::Released)
		{
			newActionFuctions.onReleased = actionFunction;
		}
		m_boundActionMappings.insert_or_assign(actionName, newActionFuctions);
	}
}

void AFInput::BindAxis(const std::string& axis, const std::function<void(float)>& fun)
{
	// Remember key to value.
	const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();
	auto foundAxis = configMappings.axisMappings.find(axis);
	if(foundAxis == configMappings.axisMappings.end())
	{
		return;
	}
	
	FAFBoundAxis boundAxis;
	boundAxis.fun = fun;
	boundAxis.keyToValue1 = foundAxis->second.keyToValue1;
	boundAxis.keyToValue2 = foundAxis->second.keyToValue2;

	m_boundAxisMappings.insert_or_assign(axis, boundAxis);
}

void AFInput::Init()
{
	// Register all configurable keys into keystate map.
	for(const auto& [action, key] : AFUtility::GetConfigurableKeys())
	{
		m_keystate.insert_or_assign(key, 0);
	}

	//BindAction("FreeViewMode", []{printf("freeview mode pressed\n"); }, EAFKeyAction::Pressed);
	//BindAction("FreeViewMode", []{printf("freeview mode released\n"); }, EAFKeyAction::Released);
	//BindAxis("FreeView_UpDown", [](float axis) {printf("%f\n", axis); });
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

	if (m_mouseLock)
	{

	}
}
