#include "AFInput.h"

#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

void AFInput::OnKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if(!m_mouseLock)
	{
		//ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mods);
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

		if(action == GLFW_PRESS)
		{
			return;
		}
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		m_mouseLock = true;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		m_mouseLock = false;
	}

	if(m_mouseLock)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if(glfwRawMouseMotionSupported())
		{
			glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

}

AFInput& AFInput::GetInstance()
{
	static AFInput input;
	return input;
}

bool AFInput::GetMouseLock()
{
	return m_mouseLock;
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
