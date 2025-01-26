#include "AFInput.h"
#include "AFGame.h"

#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

double AFInput::cursorXPos = 0.0f;
double AFInput::cursorYPos = 0.0f;

bool AFInput::mouseLock = false;

void AFInput::OnKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	bool bConsumedByWindow = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		bConsumedByWindow = true;
	}

	if(mouseLock == true)
	{
		bConsumedByWindow = true;
	}

	if(!bConsumedByWindow)
	{
		ImGui_ImplGlfw_KeyCallback(window, key, scanCode, action, mods);
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
		ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

		if(action == GLFW_PRESS)
		{
			return;
		}
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		mouseLock = true;
	}

	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		mouseLock = false;
	}

	if(mouseLock)
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
	if (!window)
	{
		return;
	}

	if (mouseLock)
	{
		AFGame& game = AFGame::GetInstance();
		game.OnScrollUpdate(xoffset, yoffset);
	}
}

void AFInput::Tick()
{
	if(!window)
	{
		return;
	}

	const bool wPress = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
	const bool aPress = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
	const bool sPress = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
	const bool dPress = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
	const bool qPress = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
	const bool ePress = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;

	const float forwardBackward = (wPress ? 1.0f : (sPress ? -1.0f : 0.0f));
	const float leftRight = (dPress ? 1.0f : (aPress ? -1.0f : 0.0f));
	const float upDown = (ePress ? 1.0f : (qPress ? -1.0f : 0.0f));

	std::map<unsigned int, float> axisInputs;
	axisInputs.insert_or_assign(0, forwardBackward);
	axisInputs.insert_or_assign(1, leftRight);
	axisInputs.insert_or_assign(2, upDown);

	AFGame& game = AFGame::GetInstance();
	game.OnAxisInput(axisInputs);
}

bool AFInput::GetMouseLock()
{
	return mouseLock;
}

void AFInput::OnScrollCallback(GLFWwindow* window, double xcursor, double ycursor)
{
	if(!mouseLock)
	{
		ImGui_ImplGlfw_CursorPosCallback(window, xcursor, ycursor);
	}

	AFGame& game = AFGame::GetInstance();
	game.OnCursorPosUpdate((cursorXPos - xcursor) * -1.0f, cursorYPos - ycursor);

	cursorXPos = xcursor;
	cursorYPos = ycursor;
}
