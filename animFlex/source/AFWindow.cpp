#include "AFWindow.h"
#include "AFInput.h"

#include <iostream>
#include <cmath>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include "glad/glad.h"
#include <GLFW/glfw3.h>

AFWindow& AFWindow::GetInstance()
{
	static AFWindow windowInstance;
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
	return glfwWindowShouldClose(Window);
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

	Window = glfwCreateWindow(800, 100, "myFlexWindow", nullptr, nullptr);
	if(!Window)
	{
		printf("%s", "window not constructed correctly!\n");

		glfwTerminate();
		return;
	}

	// Select a current OpenGL context.
	glfwMakeContextCurrent(Window);

	// Load OpenGL function pointers.
	if(!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		printf("%s", "failed to initialize glad!\n");

		glfwTerminate();
		return;
	}

	// Bind input events.
	glfwSetKeyCallback(Window, AFInput::OnKeyboardInput);
	glfwSetMouseButtonCallback(Window, AFInput::OnCursorInput);

	// Set OpenGL viewport.
	int width, height;
	glfwGetFramebufferSize(Window, &width, &height);
	glViewport(0, 0, width, height);

	m_PreviousTime = glfwGetTime();

	printf("%s", "window constructed!\n");
}

AFWindow::~AFWindow()
{
	glfwTerminate();
	printf("%s", "window destructed!\n");
}

void AFWindow::Tick_Internal(float InDeltaTime)
{
	printf("%f\n", 1.0f / InDeltaTime);
	const double time = fmod(glfwGetTime(), 1.0);
	glClearColor((float)time, (float)time, (float)time, 1.0f);
}

void AFWindow::Tick()
{
	// Calculate delta time.
	const double currentTime = glfwGetTime();
	m_DeltaTime = currentTime - m_PreviousTime;
	m_PreviousTime = currentTime;

	// Internal tick, doing all the heavy work.
	Tick_Internal(m_DeltaTime);

	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(Window);
	glfwPollEvents();
}
