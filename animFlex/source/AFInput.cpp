#include "AFInput.h"

#include <iostream>

void AFInput::OnKeyboardInput(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (!window)
	{
		return;
	}

	if (action == 1)
	{
		printf("%d\n", key);
	}
}

void AFInput::OnCursorInput(GLFWwindow* window, int button, int action, int mods)
{
	if (!window)
	{
		return;
	}

	if (action == 1)
	{
		printf("%d\n", button);
	}
}
