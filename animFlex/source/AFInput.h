#pragma once

class AFInput
{
	friend class AFApp;
	friend class AFWindow;

public:

	AFInput() = delete;
	~AFInput() = delete;

	static bool GetMouseLock();

private:

	static void Tick();

	static void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	static void OnCursorPosCallback(struct GLFWwindow* window, double xoffset, double yoffset);
	static void OnScrollCallback(struct GLFWwindow* window, double xcursor, double ycursor);

	static double cursorXPos;
	static double cursorYPos;

	static bool mouseLock;
};
