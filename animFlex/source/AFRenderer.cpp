#include "AFRenderer.h"
#include "AFGame.h"
#include "AFTimerManager.h"

#include <ostream>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

bool AFRenderer::Init(int width, int height)
{
	// Load OpenGL function pointers.
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		printf("%s", "failed to initialize glad!\n");

		glfwTerminate();
		return false;
	}

	printf("%s\n", GetOpenGLVersion());

	// Init the frame buffer with initial information, tell it how to interpret textures, tell it to create depth buffer.
	if(!m_framebuffer.Init(width, height))
	{
		return false;
	}

	// Load all render components.
	// ...

	// Set up the uniform buffer.
	m_uniformBuffer.Init();

	return true;
}

void AFRenderer::SetSize(int newWidth, int newHeight)
{
	m_framebuffer.Resize(newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void AFRenderer::Cleanup()
{
	m_framebuffer.Cleanup();
	m_userInterface.Cleanup();
}

void AFRenderer::CheckGLErrors(const char* label)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) 
	{
		printf("OpenGL Error: %u in %s", error, label);
	}
}

const GLubyte* AFRenderer::GetOpenGLVersion()
{
	return glGetString(GL_VERSION);
}

void AFRenderer::Draw(const AFSceneData& sceneData)
{
	AFGame& game = AFGame::GetInstance();

	// Start the timer to count how long does it take to draw stuff into the frame buffer.
	AFTimerManager::AFTimer drawTimer;
	drawTimer.Start();

	// Enable drawing to the frame buffer.
	m_framebuffer.Bind();

	// Set the background colour.
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// Clear colour and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable face culling to not render back of triangles.
	glEnable(GL_CULL_FACE);

	const glm::vec2& frameBufferSize = m_framebuffer.GetSize();
	// Safely create the projection matrix given screen width/height and camera's FOV.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(static_cast<float>(m_renderData.fieldOfView)),
			static_cast<float>(frameBufferSize.x) / static_cast<float>(frameBufferSize.y), 0.1f, 100.0f);
	}

	// Create the view matrix given camera's transform.
	m_viewMatrix = game.GetCamera().GetViewMatrix();

	// Upload the view and projection matrices into the uniform buffer, which will be used across shaders.
	m_uniformBuffer.UploadUBOData(m_viewMatrix, m_projectionMatrix);

	// Per object draw.

	// Tell the gpu which shader to use.
	m_basicShader.Use();
	// Bind texture object - it will be used during drawing of triangles to push the texture data to frag shader.
	m_tex.Bind();
	// Bind the vertex buffer which already contains information about vertices - their locations, color, uv mapping, etc. 
	m_vertexBuffer.Bind();
	// Draw the triangles.
	m_vertexBuffer.Draw(GL_TRIANGLES, 0, m_renderData.vertexCount);
	// Unbind the buffer with vertices.
	m_vertexBuffer.UnBind();
	// Unbind the texture object.
	m_tex.UnBind();

	// Unbind the frame buffer, we don't want to draw to it anymore.
	m_framebuffer.UnBind();

	// Draw all the contents of the frame buffer to the screen.
	m_framebuffer.DrawToScreen();

	// Check how long it took.
	m_renderData.gameDrawTime = drawTimer.Stop();

	// Start the timer for UI drawing.
	drawTimer.Start();

	// Create frame buffer for UI.
	m_userInterface.CreateFrame(m_renderData);
	// Draw the UI frame buffer.
	m_userInterface.Render();

	// Check how long it took.
	m_renderData.uiDrawTime = drawTimer.Stop();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
