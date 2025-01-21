#include "AFRenderer.h"

#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "AFGame.h"

bool AFRenderer::Init(int width, int height, GLFWwindow* window)
{
	m_renderData.window = window;

	m_renderData.width = width;
	m_renderData.height = height;

	// Load OpenGL function pointers.
	if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress))
	{
		printf("%s", "failed to initialize glad!\n");

		glfwTerminate();
		return false;
	}

	printf("%s\n", GetOpenGLVersion());

	if(!m_framebuffer.Init(width, height))
	{
		return false;
	}

	if(!m_tex.LoadTexture("content/textures/crate.png"))
	{
		return false;
	}

	m_vertexBuffer.Init();

	if(!m_basicShader.LoadShaders("content/shaders/basic.vert", "content/shaders/basic.frag"))
	{
		return false;
	}

	if (!m_changedShader.LoadShaders("content/shaders/changed.vert", "content/shaders/changed.frag"))
	{
		return false;
	}

	m_uniformBuffer.Init();

	m_userInterface.Init(m_renderData);

	return true;
}

void AFRenderer::SetSize(int newWidth, int newHeight)
{
	m_renderData.width = newWidth;
	m_renderData.height = newHeight;

	m_framebuffer.Resize(newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void AFRenderer::UploadData(const AFMesh& newMesh)
{
	m_renderData.triangleCount = newMesh.vertices.size();
	m_vertexBuffer.UploadData(newMesh);
}

void AFRenderer::Cleanup()
{
	m_basicShader.Cleanup();
	m_changedShader.Cleanup();

	m_tex.Cleanup();
	m_vertexBuffer.Cleanup();
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

void AFRenderer::Draw()
{
	AFGame& game = AFGame::GetInstance();
	const bool testState = game.GetTestState();

	m_framebuffer.Bind();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_CULL_FACE);

	glm::vec3 cameraPosition = glm::vec3(0.4f, 0.3f, 1.0f);
	glm::vec3 cameraLookAtPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUpVector = glm::vec3(0.0f, 1.0f, 0.0f);

	m_projectionMatrix = glm::perspective(glm::radians(90.0f), static_cast<float>(m_renderData.width) / static_cast<float>(m_renderData.height), 0.1f, 100.0f);
	glm::mat4 view = glm::mat4(1.0f);

	const float t = static_cast<float>(glfwGetTime());
	if(testState)
	{
		m_changedShader.Use();
		view = glm::rotate(glm::mat4(1.0f), t, glm::vec3(0.0f, 0.0f, 1.0f));

	}
	else
	{
		m_basicShader.Use();
		view = glm::rotate(glm::mat4(1.0f), -t, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	m_viewMatrix = glm::lookAt(cameraPosition, cameraLookAtPosition, cameraUpVector) * view;
	m_uniformBuffer.UploadUBOData(m_viewMatrix, m_projectionMatrix);
	
	m_tex.Bind();
	m_vertexBuffer.Bind();
	m_vertexBuffer.Draw(GL_TRIANGLES, 0, m_renderData.triangleCount);
	m_vertexBuffer.UnBind();
	m_tex.UnBind();
	m_framebuffer.UnBind();

	m_framebuffer.DrawToScreen();

	m_userInterface.CreateFrame(m_renderData);
	m_userInterface.Render();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
