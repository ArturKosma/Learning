#include "AFRenderer.h"
#include "AFCamera.h"

#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "AFRenderComponent.h"
#include "AFUtility.h"

bool AFRenderer::Init(int width, int height)
{
	printf("%s\n", GetOpenGLVersion());

	// Init the frame buffer with initial information, tell it how to interpret textures, tell it to create depth buffer.
	if(!m_framebuffer.Init(width, height))
	{
		printf("%s\n", "Fail on frame buffer Init().");
		return false;
	}

	// Set up the uniform buffer.
	m_uniformBuffer.Init();

	return true;
}

bool AFRenderer::TESTInit(int width, int height)
{
	printf("%s\n", GetOpenGLVersion());

	// Init the frame buffer with initial information, tell it how to interpret textures, tell it to create depth buffer.
	if (!m_framebuffer.TESTInit(width, height))
	{
		printf("%s\n", "Fail on frame buffer Init().");
		return false;
	}

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

void AFRenderer::TESTDraw(const AFSceneData& sceneData)
{
	// Make sure closer objects are drawn before farther.
	glDepthFunc(GL_LESS);

	// Enable drawing to the frame buffer.
	m_framebuffer.TESTBind();

	// Set the background colour.
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Clear colour and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable face culling to not render back of triangles.
	glEnable(GL_CULL_FACE);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	const glm::vec2& frameBufferSize = m_framebuffer.GetSize();
	// Safely create the projection matrix given screen width/height and camera's FOV.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(static_cast<float>(sceneData.activeCamera->GetCameraComponent()->GetCameraProperties().fieldOfView)),
			frameBufferSize.x / frameBufferSize.y, 0.1f, 100.0f);

		m_orthoMatrix = glm::ortho(0.0f, frameBufferSize.x, frameBufferSize.y, 0.0f, -1.0f, 1.0f);
	}

	// Create the view matrix given camera's transform.
	m_viewMatrix = sceneData.activeCamera->GetCameraComponent()->GetViewMatrix();

	// Per object draw.
	for(const AFActor* const sceneActor : sceneData.sceneActors)
	{
		// Per component draw.
		for(const AFComponent* const component : sceneActor->GetComponents())
		{
			const AFRenderComponent* const renderComponent = dynamic_cast<const AFRenderComponent*>(component);
			if(!renderComponent)
			{
				continue;
			}

			// Upload the view, projection and model matrices into the uniform buffer, which will be used across shaders.
			m_uniformBuffer.UploadMatrices(m_viewMatrix, m_projectionMatrix, renderComponent->GetWorldTransform());
			m_uniformBuffer.UploadCamera(sceneData.activeCamera->GetCameraComponent()->GetWorldTransform());

			// Draw.
			renderComponent->Draw();
		}
	}

	// Unbind the frame buffer, we don't want to draw to it anymore.
	m_framebuffer.UnBind();

	// Draw all the contents of the frame buffer to the screen.
	m_framebuffer.TESTDrawToScreen();

	DebugOpenGL();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}

void AFRenderer::Draw(const AFSceneData& sceneData)
{
	// Make sure closer objects are drawn before farther.
	glDepthFunc(GL_LESS);

	// Enable drawing to the frame buffer.
	m_framebuffer.Bind();

	// Set the background colour.
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Clear colour and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable face culling to not render back of triangles.
	glEnable(GL_CULL_FACE);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	const glm::vec2& frameBufferSize = m_framebuffer.GetSize();
	// Safely create the projection matrix given screen width/height and camera's FOV.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(static_cast<float>(sceneData.activeCamera->GetCameraComponent()->GetCameraProperties().fieldOfView)),
			frameBufferSize.x / frameBufferSize.y, 0.1f, 100.0f);

		m_orthoMatrix = glm::ortho(0.0f, frameBufferSize.x, frameBufferSize.y, 0.0f, -1.0f, 1.0f);
	}

	// Create the view matrix given camera's transform.
	m_viewMatrix = sceneData.activeCamera->GetCameraComponent()->GetViewMatrix();

	// Per object draw.
	for (const AFActor* const sceneActor : sceneData.sceneActors)
	{
		// Per component draw.
		for (const AFComponent* const component : sceneActor->GetComponents())
		{
			const AFRenderComponent* const renderComponent = dynamic_cast<const AFRenderComponent*>(component);
			if (!renderComponent)
			{
				continue;
			}

			// Upload the view, projection and model matrices into the uniform buffer, which will be used across shaders.
			m_uniformBuffer.UploadMatrices(m_viewMatrix, m_projectionMatrix, renderComponent->GetWorldTransform());
			m_uniformBuffer.UploadCamera(sceneData.activeCamera->GetCameraComponent()->GetWorldTransform());

			// Draw.
			renderComponent->Draw();
		}
	}

	// Unbind the frame buffer, we don't want to draw to it anymore.
	m_framebuffer.UnBind();

	// Draw all the contents of the frame buffer to the screen.
	m_framebuffer.DrawToScreen();

	DebugOpenGL();
}
