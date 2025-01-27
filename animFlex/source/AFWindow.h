#pragma once

#include <functional>
#include <GLFW/glfw3.h>

class AFWindow
{
	friend class AFApp;

public:

	GLFWwindow* GetGLFWWindow() const;

private:

	bool Init(int initWidth, int initHeight);

	int GetWidth() const;
	int GetHeight() const;

	void SwapBuffers();
	void PollEvents();

	void OnWindowResize(int newWidth, int newHeight);
	bool ShouldShutdown();

	AFWindow();
	~AFWindow();

	GLFWwindow* m_window = nullptr;
	std::function<void(int, int)> m_resizeCallback;
};
