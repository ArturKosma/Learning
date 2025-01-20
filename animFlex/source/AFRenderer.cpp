#include "AFRenderer.h"

#include <ostream>

#include "AFGame.h"

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

	return true;
}

void AFRenderer::SetSize(int newWidth, int newHeight)
{
	m_framebuffer.Resize(newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void AFRenderer::UploadData(const AFMesh& newMesh)
{
	m_triangleCount = newMesh.vertices.size();
	m_vertexBuffer.UploadData(newMesh);
}

void AFRenderer::Cleanup()
{
	m_basicShader.Cleanup();
	m_changedShader.Cleanup();

	m_tex.Cleanup();
	m_vertexBuffer.Cleanup();
	m_framebuffer.Cleanup();
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

	if(testState)
	{
		m_changedShader.Use();
	}
	else
	{
		m_basicShader.Use();
	}
	
	m_tex.Bind();
	m_vertexBuffer.Bind();
	m_vertexBuffer.Draw(GL_TRIANGLES, 0, m_triangleCount);
	m_vertexBuffer.UnBind();
	m_tex.UnBind();
	m_framebuffer.UnBind();

	m_framebuffer.DrawToScreen();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
