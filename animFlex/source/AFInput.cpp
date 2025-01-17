#include "AFInput.h"

#include <iostream>
#include <GLFW/glfw3.h>

void AFInput::OnKeyboardInput(GLFWwindow* InWindow, int InKey, int InScanCode, int InAction, int InMods)
{
	if (!InWindow)
	{
		return;
	}

	if (InAction == GLFW_PRESS)
	{
		printf("%d\n", InKey);
	}
}

void AFInput::OnCursorInput(GLFWwindow* InWindow, int InButton, int InAction, int InMods)
{
	if (!InWindow)
	{
		return;
	}

	if (InAction == GLFW_PRESS)
	{
		printf("%d\n", InButton);
	}
}
