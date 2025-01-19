#include "AFRenderer.h"

#include <cmath>
#include <GLFW/glfw3.h>

void AFRenderer::TickRender(float deltaTime)
{
	const double time = fmod(glfwGetTime(), 1.0);
	glClearColor((float)time, (float)time, (float)time, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void AFRenderer::OnWindowResize(int newWidth, int newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
