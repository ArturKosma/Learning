#include "AFWindow.h"
#include "AFInput.h"

#include <iostream>

#include "AFUtility.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

bool AFWindow::Init(int initWidth, int initHeight)
{
	if (!glfwInit())
	{
		printf("%s", "glfw not init correctly!\n");

		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	// Request MSAA.
	//glfwWindowHint(GLFW_SAMPLES, 4);

	m_window = glfwCreateWindow(initWidth, initHeight, "myFlexWindow", nullptr, nullptr);
	if (!m_window)
	{
		printf("%s", "window not constructed correctly!\n");

		glfwTerminate();
		return false;
	}

	// Select a current OpenGL context.
	glfwMakeContextCurrent(m_window);

	// Store the window pointer in the internals of GLFW to access in bindings.
	glfwSetWindowUserPointer(m_window, this);

	// Load OpenGL function pointers.
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		printf("%s\n", "failed to initialize glad!");

		glfwTerminate();
		return false;
	}

	// Bind input events.
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scanCode, int action, int mods)
	{
		AFInput::GetInstance().OnKeyCallback(window, key, scanCode, action, mods);
	});
	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		AFInput::GetInstance().OnMouseButtonCallback(window, button, action, mods);
	});
	glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
	{
		AFInput::GetInstance().OnCursorPosCallback(window, xoffset, yoffset);
	});
	glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xcursor, double ycursor)
	{
		AFInput::GetInstance().OnScrollCallback(window, xcursor, ycursor);
	});

	// Bind window resize.
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int newWidth, int newHeight)
		{
			AFWindow* windowInstance = static_cast<AFWindow*>(glfwGetWindowUserPointer(window));
			if (windowInstance)
			{
				windowInstance->OnWindowResize(newWidth, newHeight);
			}
		});

#ifdef __EMSCRIPTEN__

	// Emscripten resizes the window by html style properties.

#else

	// Make the window cover whole screen on desktop.

	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	if (!primaryMonitor)
	{
		printf("Failed to get the primary monitor!\n");
		glfwTerminate();
		return false;
	}

	// Get the monitor's video mode (resolution, refresh rate, etc.).
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
	if (!videoMode)
	{
		printf("Failed to get video mode for the primary monitor!\n");
		glfwTerminate();
		return false;
	}

	const int width = videoMode->width;
	const int height = videoMode->height;

	glfwSetWindowSize(m_window, width, height);
	ClearOpenGLErrors();
	DebugOpenGL();

#endif
	return true;
}

GLFWwindow* AFWindow::GetGLFWWindow() const
{
	return m_window;
}

int AFWindow::GetWidth() const
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(m_window, &width, &height);

	return width;
}

int AFWindow::GetHeight() const
{
	int width = 0;
	int height = 0;
	glfwGetWindowSize(m_window, &width, &height);

	return height;
}

bool AFWindow::ShouldShutdown()
{
	const bool shouldShutdown = glfwWindowShouldClose(m_window);
	return shouldShutdown;
}

AFWindow::AFWindow()
{

}

AFWindow::~AFWindow()
{
	glfwTerminate();
}

void AFWindow::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

void AFWindow::PollEvents()
{
	glfwPollEvents();
}

void AFWindow::OnWindowResize(int newWidth, int newHeight)
{
	glfwSetWindowSize(m_window, newWidth, newHeight);
	m_resizeCallback(newWidth, newHeight);
}
