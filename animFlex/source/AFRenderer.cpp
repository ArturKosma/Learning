#include "AFRenderer.h"
#include "AFCamera.h"

#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "AFMath.h"
#include "AFRenderComponent.h"

bool AFRenderer::Init(int width, int height)
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

	// Set up the background shader.
	m_backgroundShader.SetVertexShader("content/shaders/background.vert");
	m_backgroundShader.SetFragmentShader("content/shaders/background.frag");
	if (!m_backgroundShader.LoadShaders())
	{
		printf("%s\n", "Fail on background shader load.");
		return false;
	}

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

void AFRenderer::Draw(const AFSceneData& sceneData)
{
	AFCamera* camera = sceneData.activeCamera;
	if(!camera)
	{
		return;
	}

	AFCameraComponent* cameraComp = camera->GetCameraComponent();
	if(!cameraComp)
	{
		return;
	}

	AFCameraMovementComponent* cameraMov = camera->GetMovementComponent();
	if(!cameraMov)
	{
		return;
	}

	const glm::vec2& frameBufferSize = m_framebuffer.GetSize();

	// Safely create the projection matrix given screen width/height and camera's FOV.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(static_cast<float>(cameraComp->GetCameraProperties().fieldOfView)),
			frameBufferSize.x / frameBufferSize.y, 0.1f, 100.0f);

		m_orthoMatrix = glm::ortho(0.0f, frameBufferSize.x, frameBufferSize.y, 0.0f, -1.0f, 1.0f);
	}

	// Create the view matrix given camera's transform.
	m_viewMatrix = cameraComp->GetViewMatrix();

	// Set the background colour.
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Make sure closer objects are drawn before farther.
	glDepthFunc(GL_LESS);

	// Enable drawing to the frame buffer.
	m_framebuffer.Bind();

	// Clear colour and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable depth for background drawing.
	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	// Upload inverse of view and projection, purely for the background shader.
	m_uniformBuffer.UploadViewProjection(glm::inverse(m_viewMatrix), glm::inverse(m_projectionMatrix));

	// Use background shader.
	m_backgroundShader.Use();

	// Resolution, projection and view uniforms for the background shader.
	GLint resolutionUniformLoc = glGetUniformLocation(m_backgroundShader.GetProgram(), "u_Resolution");
	glUniform2f(resolutionUniformLoc, static_cast<float>(frameBufferSize.x), static_cast<float>(frameBufferSize.y));
	GLint viewProjectionIndex = glGetUniformBlockIndex(m_backgroundShader.GetProgram(), "InverseViewProjection");
	glUniformBlockBinding(m_backgroundShader.GetProgram(), viewProjectionIndex, 0);
	GLint cameraPositionIndex = glGetUniformLocation(m_backgroundShader.GetProgram(), "u_CameraPos");
	glUniform3f(cameraPositionIndex, cameraComp->GetWorldLocation().x, cameraComp->GetWorldLocation().y, cameraComp->GetWorldLocation().z);

	// Draw 3 non defined points.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Enable depth for rest of scene.
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

	// Enable face culling to not render back of triangles.
	glEnable(GL_CULL_FACE);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);

	// Upload proper view, projection and camera matrices.
	m_uniformBuffer.UploadViewProjection(m_viewMatrix, m_projectionMatrix);
	m_uniformBuffer.UploadCamera(cameraComp->GetWorldTransform());

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
			// Upload the model matrix.
			m_uniformBuffer.UploadTransform(renderComponent->GetWorldTransform());

			// Draw.
			renderComponent->Draw();
		}
	}

	// Unbind the frame buffer, we don't want to draw to it anymore.
	m_framebuffer.UnBind();

	// Draw all the contents of the frame buffer to the screen.
	m_framebuffer.DrawToScreen();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
