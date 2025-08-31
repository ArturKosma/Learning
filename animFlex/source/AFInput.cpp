#include "AFInput.h"
#include "AFConfig.h"
#include <glad/glad.h>
#include "AFUtility.h"
#include <algorithm>
#define GLFW_INCLUDE_NONE
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

	// Upon PollEvents() this function can be called multiple times, so we just save the cursor pos here and retrieve the delta in Tick().
	m_cursorNewXPos = posX;
	m_cursorNewYPos = posY;
}

void AFInput::Tick()
{
	UpdateCursorPosState();
	UpdateStrokeState();

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

	auto foundMapping = GetInstance().m_boundActionMappings.find(actionName);
	if(foundMapping != GetInstance().m_boundActionMappings.end())
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

	GetInstance().m_boundActionMappings.insert_or_assign(actionName, actionWrapper);
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

	GetInstance().m_boundAxisMappings.insert_or_assign(axis, boundAxis);
}

void AFInput::UnbindAllInputs()
{
	GetInstance().m_boundActionMappings.clear();
	GetInstance().m_boundAxisMappings.clear();
}

bool AFInput::GetMouseDown()
{
	bool down = false;
	if(m_keystate.find(GLFW_MOUSE_BUTTON_1) != m_keystate.end())
	{
		down = m_keystate.at(GLFW_MOUSE_BUTTON_1) == 1.0f;
	}

	return down;
}

glm::vec2 AFInput::GetCursorPos()
{
	return {m_cursorNewXPos, m_cursorNewYPos };
}

bool AFInput::GetTouchDown(int touchIndex)
{
	const bool down = m_touchstate.size() == 1;

	return down;
}

glm::vec2 AFInput::GetTouchPos(int touchIndex)
{
	const size_t count = m_touchstate.size();

	// Single stroke.
	if (count == 1)
	{
		// Find delta and update last frame.
		auto& [id, singleTouch] = *m_touchstate.begin();
		return singleTouch.newLocation;
	}

	return { 0.0f, 0.0f };
}

void AFInput::Init(GLFWwindow* window)
{
	GetInstance().m_window = window;

	// Register all configurable keys into keystate map.
	for(const auto& [action, key] : AFUtility::GetConfigurableKeys())
	{
		GetInstance().m_keystate.insert_or_assign(key, 0.0f);
	}
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

	if(yscroll > 0.0)
	{
		OnMouseButtonCallback(window, 1005, 1, 0);
	}
	if (yscroll < 0.0)
	{
		OnMouseButtonCallback(window, 1006, 1, 0);
	}
}

void AFInput::RegisterTouch(int id, const glm::ivec2& location)
{
	m_touchstate[id] = FAFTouch(location);

	// Treat first touch as regular mouse click.
	if(m_touchstate.size() == 1)
	{
		const AFConfig::FAFConfigMappings& configMappings = AFConfig::GetInstance().GetConfigMappings();
		for (const auto& [configAction, configKey] : configMappings.actionMappings)
		{
			if (configKey == GLFW_MOUSE_BUTTON_1)
			{
				const auto& foundBound = m_boundActionMappings.find(configAction);
				if (foundBound != m_boundActionMappings.end())
				{
					for (const std::function<void()>& pressedFunctor : foundBound->second.onPressedFunctors)
					{
						pressedFunctor();
					}
				}
			}
		}
	}
}

void AFInput::UnregisterTouch(int id)
{
	m_touchstate.erase(id);
}

#ifdef __EMSCRIPTEN__
void AFInput::OnTouchStart(int eventType, const EmscriptenTouchEvent* e)
{
	if(!e)
	{
		return;
	}

	for (int i = 0; i < e->numTouches; ++i)
	{
		const EmscriptenTouchPoint& point = e->touches[i];
		if(point.isChanged)
		{
			const bool alreadyRegistered = m_touchstate.find(point.identifier) != m_touchstate.end();
			if(!alreadyRegistered)
			{
				RegisterTouch(point.identifier, { point.clientX, point.clientY });
			}
		}
	}
}

void AFInput::OnTouchMove(int eventType, const EmscriptenTouchEvent* e)
{
	if (!e)
	{
		return;
	}

	for (int i = 0; i < e->numTouches; ++i)
	{
		const EmscriptenTouchPoint& point = e->touches[i];
		if (point.isChanged)
		{
			auto it = m_touchstate.find(point.identifier);
			if(it != m_touchstate.end())
			{
				it->second.newLocation = { point.clientX, point.clientY };
			}
		}
	}
}

void AFInput::OnTouchEnd(int eventType, const EmscriptenTouchEvent* e)
{
	if (!e)
	{
		return;
	}

	for (int i = 0; i < e->numTouches; ++i)
	{
		const EmscriptenTouchPoint& point = e->touches[i];
		if (point.isChanged)
		{
			UnregisterTouch(point.identifier);
		}
	}
}
#endif

void AFInput::UpdateCursorPosState()
{
	double cursorPosXDelta = m_cursorNewXPos - m_cursorOldXPos;
	double cursorPosYDelta = m_cursorNewYPos - m_cursorOldYPos;
	m_cursorOldXPos = m_cursorNewXPos;
	m_cursorOldYPos = m_cursorNewYPos;

	// Different delta calculation for raw mouse motion to prevent opengl bumps.
	if (glfwRawMouseMotionSupported() &&
		glfwGetInputMode(m_window, GLFW_RAW_MOUSE_MOTION) == GLFW_TRUE)
	{
		double x, y;
		glfwGetCursorPos(m_window, &x, &y);

		int w, h;
		glfwGetWindowSize(m_window, &w, &h);
		double centerX = w * 0.5;
		double centerY = h * 0.5;

		cursorPosXDelta = x - centerX;
		cursorPosYDelta = y - centerY;

		// Warp cursor back to center.
		glfwSetCursorPos(m_window, centerX, centerY);
	}

	const double mouseTurnRight = cursorPosXDelta > 0.0 ? abs(static_cast<float>(cursorPosXDelta)) : 0.0f;
	const double mouseTurnLeft = cursorPosXDelta < 0.0 ? abs(static_cast<float>(cursorPosXDelta)) : 0.0f;
	const double mouseTiltUp = cursorPosYDelta > 0.0 ? abs(static_cast<float>(cursorPosYDelta)) : 0.0f;
	const double mouseTiltDown = cursorPosYDelta < 0.0 ? abs(static_cast<float>(cursorPosYDelta)) : 0.0f;

	// Update the mouse turn/tile state.
	m_keystate.insert_or_assign(1001, static_cast<float>(mouseTurnRight)); // Mouse turn right.
	m_keystate.insert_or_assign(1002, static_cast<float>(mouseTurnLeft)); // Mouse turn left.
	m_keystate.insert_or_assign(1003, static_cast<float>(mouseTiltUp)); // Mouse tilt up.
	m_keystate.insert_or_assign(1004, static_cast<float>(mouseTiltDown)); // Mouse tilt down.
}

void AFInput::UpdateStrokeState()
{
	// Initially we assume there is no stroke at all.
	// This is because we don't want to update both zoom and turn together, for example.
	m_keystate.insert_or_assign(1007, static_cast<float>(0)); // Stroke turn right.
	m_keystate.insert_or_assign(1008, static_cast<float>(0)); // Stroke turn left.
	m_keystate.insert_or_assign(1009, static_cast<float>(0)); // Stroke tilt up.
	m_keystate.insert_or_assign(1010, static_cast<float>(0)); // Stroke tilt down.
	m_keystate.insert_or_assign(1011, static_cast<float>(0)); // Stroke zoom in.
	m_keystate.insert_or_assign(1012, static_cast<float>(0)); // Stroke zoom out.

	// How many active touches there is?
	const size_t count = m_touchstate.size();

	// Single stroke.
	if(count == 1)
	{
		// Find delta and update last frame.
		auto& [id, singleTouch] = *m_touchstate.begin();
		const glm::ivec2 delta = singleTouch.newLocation - singleTouch.lastFrame;
		singleTouch.lastFrame = singleTouch.newLocation;

		const double strokeTurnRight = delta.x > 0 ? abs(static_cast<float>(delta.x)) : 0.0f;
		const double strokeTurnLeft = delta.x < 0 ? abs(static_cast<float>(delta.x)) : 0.0f;
		const double strokeTiltUp = delta.y > 0 ? abs(static_cast<float>(delta.y)) : 0.0f;
		const double strokeTiltDown = delta.y < 0 ? abs(static_cast<float>(delta.y)) : 0.0f;

		// Update the single stroke turn/tilt state.
		m_keystate.insert_or_assign(1007, static_cast<float>(strokeTurnRight)); // Stroke turn right.
		m_keystate.insert_or_assign(1008, static_cast<float>(strokeTurnLeft)); // Stroke turn left.
		m_keystate.insert_or_assign(1009, static_cast<float>(strokeTiltUp)); // Stroke tilt up.
		m_keystate.insert_or_assign(1010, static_cast<float>(strokeTiltDown)); // Stroke tilt down.
	}
	// Double stroke.
	else if(count == 2)
	{
		std::vector<glm::vec2> oldLocations = { glm::vec2(0), glm::vec2(0) };
		std::vector<glm::vec2> newLocations = { glm::vec2(0), glm::vec2(0) };
		std::vector<float> magnitudes = { 0.0f, 0.0f };
		size_t idx = 0;
		for(auto& [id, touch] : m_touchstate)
		{
			const float magnitude = glm::distance(glm::vec2(touch.newLocation), glm::vec2(touch.lastFrame));
			if(magnitude == 0.0f)
			{
				// Both touches have to move.
				return;
			}

			oldLocations[idx] = touch.lastFrame;
			newLocations[idx] = touch.newLocation;
			magnitudes[idx] = magnitude;

			touch.lastFrame = touch.newLocation;
			++idx;
		}

		const float oldLocationsDist = glm::distance(oldLocations[0], oldLocations[1]);
		const float newLocationsDist = glm::distance(newLocations[0], newLocations[1]);
		const float avgMagnitude = (magnitudes[0] + magnitudes[1]) / 2.0f;

		const float strokeZoomIn = newLocationsDist > oldLocationsDist ? avgMagnitude : 0.0f;
		const float strokeZoomOut = newLocationsDist < oldLocationsDist ? avgMagnitude : 0.0f;

		// Update the zoom in/out stroke state.
		m_keystate.insert_or_assign(1011, static_cast<float>(strokeZoomIn)); // Stroke zoom in.
		m_keystate.insert_or_assign(1012, static_cast<float>(strokeZoomOut)); // Stroke zoom out.
	}
}
