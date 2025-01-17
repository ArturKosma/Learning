#pragma once

class AFInput
{
public:

	AFInput() = delete;
	~AFInput() = delete;

	static void OnKeyboardInput(struct GLFWwindow* InWindow, int InKey, int InScanCode, int InAction, int InMods);
	static void OnCursorInput(struct GLFWwindow* InWindow, int InButton, int InAction, int InMods);
};
