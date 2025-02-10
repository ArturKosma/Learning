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
	if (!m_framebuffer.Init(width, height))
	{
		printf("%s\n", "Fail on frame buffer Init().");
		return false;
	}

	// Set up the uniform buffer.
	m_uniformBuffer.Init();

	// Make sure closer objects are drawn before farther.
	glDepthFunc(GL_LESS);

	// Enable face culling to not render back of triangles.
	glEnable(GL_CULL_FACE);

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

	// Fetch framebuffer size and camera properties.
	const glm::vec2& frameBufferSize = m_framebuffer.GetSize();
	const AFCameraProperties& cameraProps = cameraComp->GetCameraProperties();
	const float fov = static_cast<float>(cameraProps.fieldOfView);
	const float near = cameraProps.near;
	const float far = cameraProps.far;

	// Safely create the projection matrix given screen width/height.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(fov), frameBufferSize.x / frameBufferSize.y, near, far);
	}

	// Enable drawing to the frame buffer.
	m_framebuffer.Bind();

	// Set the background colour.
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Clear colour and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create the view matrix given camera's transform.
	m_viewMatrix = cameraComp->GetViewMatrix();

	// Inverse view & projection upload.
	m_uniformBuffer.UploadInverseViewProjection(glm::inverse(m_viewMatrix), glm::inverse(m_projectionMatrix));

	// Resolution upload.
	glm::mat4 resolutionMatrix;
	resolutionMatrix[0][0] = frameBufferSize.x;
	resolutionMatrix[0][1] = frameBufferSize.y;
	m_uniformBuffer.UploadResolution(resolutionMatrix);

	// View & projection upload.
	m_uniformBuffer.UploadViewProjection(m_viewMatrix, m_projectionMatrix);

	// Camera transform upload.
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
	m_framebuffer.DrawToScreen(sceneData);
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
