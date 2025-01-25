#include "AFWindow.h"
#include "AFInput.h"
#include "AFRenderer.h"
#include "AFGame.h"
#include "AFUtil.h"

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

float AFWindow::deltaTime = 0.0f;

AFWindow& AFWindow::GetInstance()
{
	static AFWindow windowInstance;
	return windowInstance;
}

void AFWindow::StartLoop()
{
#ifdef __EMSCRIPTEN__
	printf("Running on Emscripten.\n");

	// Set the web main loop to run at requestAnimationFrame() fps.
	emscripten_set_main_loop_arg([](void* InWindowPtr)
		{
			AFWindow* windowPtr = static_cast<AFWindow*>(InWindowPtr);
			windowPtr->Tick();
		}, this, 0, true);

#else
	printf("Not running on Emscripten.\n");

	while(!ShouldShutdown())
	{
		Tick();
	}
#endif
}

bool AFWindow::ShouldShutdown()
{
	return glfwWindowShouldClose(m_window);
}

AFWindow::AFWindow()
{
	if (!glfwInit())
	{
		printf("%s", "glfw not init correctly!\n");

		return;
	}

	// Set OpenGL ES 3.0 context.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	const int initWidth = 800;
	const int initHeight = 600;

	m_window = glfwCreateWindow(initWidth, initHeight, "myFlexWindow", nullptr, nullptr);
	if(!m_window)
	{
		printf("%s", "window not constructed correctly!\n");

		glfwTerminate();
		return;
	}

	// Select a current OpenGL context.
	glfwMakeContextCurrent(m_window);

	// Renderer init.
	if(!m_renderer.Init(initWidth, initHeight, m_window))
	{
		printf("%s", "renderer init failed!\n");
	}

	// Temporary model.
	m_model.Init();

	// Upload temporary model.
	m_renderer.UploadData(m_model.GetVertexData());

	// Store the window pointer in the internals of GLFW to access in bindings.
	glfwSetWindowUserPointer(m_window, this);

	// Bind input events.
	glfwSetKeyCallback(m_window, AFInput::OnKeyboardInput);
	glfwSetMouseButtonCallback(m_window, AFInput::OnCursorInput);
	glfwSetCursorPosCallback(m_window, AFInput::OnCursorUpdate);

	// Bind window resize.
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* InWindow, int InNewWidth, int InNewHeight)
	{
			AFWindow* windowInstance = static_cast<AFWindow*>(glfwGetWindowUserPointer(InWindow));
			if(windowInstance)
			{
				windowInstance->OnWindowResize(InNewWidth, InNewHeight);
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
		return;
	}

	// Get the monitor's video mode (resolution, refresh rate, etc.).
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
	if (!videoMode)
	{
		printf("Failed to get video mode for the primary monitor!\n");
		glfwTerminate();
		return;
	}

	const int width = videoMode->width;
	const int height = videoMode->height;

	glfwSetWindowSize(m_window, width, height);

#endif

	// Create the game.
	AFGame& game = AFGame::GetInstance();
	game.Init();

	m_previousTime = static_cast<float>(glfwGetTime());
}

AFWindow::~AFWindow()
{
	glfwTerminate();
}

void AFWindow::Tick_Internal(float deltaTime)
{
	AFGame& game = AFGame::GetInstance();
	game.Tick(deltaTime);

	m_renderer.Draw();
}

void AFWindow::Tick()
{
	// Calculate delta time.
	const float currentTime = static_cast<float>(glfwGetTime());
	deltaTime = currentTime - m_previousTime;
	m_previousTime = currentTime;

	AFUtil::deltaTime = deltaTime;

	AFInput::Tick(m_window);

	// Internal tick, doing all the heavy work.
	Tick_Internal(deltaTime);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void AFWindow::OnWindowResize(int newWidth, int newHeight)
{
	glfwSetWindowSize(m_window, newWidth, newHeight);
	m_renderer.SetSize(newWidth, newHeight);
}
