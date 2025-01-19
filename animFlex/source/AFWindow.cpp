#include "AFWindow.h"
#include "AFInput.h"
#include "AFRenderer.h"

#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

AFWindow& AFWindow::GetInstance()
{
	static AFWindow windowInstance;;
	return windowInstance;
}

void AFWindow::StartLoop()
{
#ifdef __EMSCRIPTEN__
	printf("Running on Emscripten!!!\n");

	// Set the web main loop to run at requestAnimationFrame fps.
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

	m_window = glfwCreateWindow(800, 600, "myFlexWindow", nullptr, nullptr);
	if(!m_window)
	{
		printf("%s", "window not constructed correctly!\n");

		glfwTerminate();
		return;
	}

	// Select a current OpenGL context.
	glfwMakeContextCurrent(m_window);

	// Load OpenGL function pointers.
	if(!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		printf("%s", "failed to initialize glad!\n");

		glfwTerminate();
		return;
	}

	// Store the class instance in the window's user pointer
	glfwSetWindowUserPointer(m_window, this);

	// Bind input events.
	glfwSetKeyCallback(m_window, AFInput::OnKeyboardInput);
	glfwSetMouseButtonCallback(m_window, AFInput::OnCursorInput);

	// Bind window resize.
	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* InWindow, int InNewWidth, int InNewHeight)
	{
			AFWindow* windowInstance = static_cast<AFWindow*>(glfwGetWindowUserPointer(InWindow));
			if(windowInstance)
			{
				windowInstance->OnWindowResize(InNewWidth, InNewHeight);
			}
	});

	// Make the window cover whole screen.
	int width, height;
#ifdef __EMSCRIPTEN__

	/*width = EM_ASM_INT({
	return document.documentElement.clientWidth || window.innerWidth;
		}) - 200;
	height = EM_ASM_INT({
	return document.documentElement.clientHeight || window.innerHeight;
		}) - 200;*/

#else

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

	width = videoMode->width;
	height = videoMode->height;

	glfwSetWindowSize(m_window, width, height);

#endif

	m_previousTime = glfwGetTime();
	printf("%s", "window constructed!\n");
}

AFWindow::~AFWindow()
{
	glfwTerminate();
	printf("%s", "window destructed!\n");
}

void AFWindow::Tick_Internal(float deltaTime)
{
	//printf("%f\n", 1.0f / deltaTime);

	m_renderer.TickRender(deltaTime);
}

void AFWindow::Tick()
{
	// Calculate delta time.
	const double currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_previousTime;
	m_previousTime = currentTime;

	// Internal tick, doing all the heavy work.
	Tick_Internal(m_deltaTime);

	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void AFWindow::OnWindowResize(int newWidth, int newHeight)
{
	glfwSetWindowSize(m_window, newWidth, newHeight);
	m_renderer.OnWindowResize(newWidth, newHeight);
}
