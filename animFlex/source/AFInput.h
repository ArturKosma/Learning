#pragma once

class AFInput
{
	friend class AFWindow;

public:

	static void Tick(struct GLFWwindow* window);

	AFInput() = delete;
	~AFInput() = delete;

	static bool GetMouseLock();

private:

	static void OnKeyboardInput(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void OnCursorInput(struct GLFWwindow* window, int button, int action, int mods);
	static void OnCursorUpdate(struct GLFWwindow* window, double cursorX, double cursorY);

	static double cursorXPos;
	static double cursorYPos;

	static bool mouseLock;
};
