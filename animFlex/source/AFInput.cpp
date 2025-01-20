#include "AFInput.h"
#include "AFGame.h"

#include <iostream>
#include <GLFW/glfw3.h>

void AFInput::OnKeyboardInput(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	if (action == GLFW_PRESS)
	{
		printf("Key pressed: %d\n", key);

		if (key == GLFW_KEY_ESCAPE)
		{
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}
}

void AFInput::OnCursorInput(GLFWwindow* window, int button, int action, int mods)
{
	if (!window)
	{
		return;
	}

	AFGame& game = AFGame::GetInstance();
	game.OnInput(action == 1);
}
