#pragma once

class AFInput
{
public:

	AFInput() = delete;
	~AFInput() = delete;

	static void OnKeyboardInput(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void OnCursorInput(struct GLFWwindow* window, int button, int action, int mods);
};
