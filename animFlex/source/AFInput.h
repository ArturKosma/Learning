#pragma once

class AFInput
{
	friend class AFApp;
	friend class AFWindow;

public:

	static AFInput& GetInstance();

	bool GetMouseLock();

private:

	AFInput();
	~AFInput();

	void Tick();

	void OnKeyCallback(struct GLFWwindow* window, int key, int scanCode, int action, int mods);
	void OnMouseButtonCallback(struct GLFWwindow* window, int button, int action, int mods);
	void OnCursorPosCallback(struct GLFWwindow* window, double xoffset, double yoffset);
	void OnScrollCallback(struct GLFWwindow* window, double xscroll, double yscroll);

	double m_cursorXPos = 0.0f;
	double m_cursorYPos = 0.0f;

	bool m_mouseLock = false;
};
