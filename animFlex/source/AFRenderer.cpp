#include "AFRenderer.h"

#include <iostream>

#include "AFCamera.h"
#include "AFMath.h"
#include "AFRenderComponent.h"
#include "AFUIRenderComponent.h"
#include "AFUtility.h"
#include "AFUI.h"

#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "IAFPickerInterface.h"

bool AFRenderer::Init(int width, int height)
{
	printf("%s\n", GetOpenGLVersion());

	// Init the frame buffer with initial information, tell it how to interpret textures, tell it to create depth buffer.
	if (!m_framebufferMS.Init(width, height))
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
	m_framebufferMS.Resize(newWidth, newHeight);
	m_framebufferIdPicker.Resize(newWidth, newHeight);
	glViewport(0, 0, newWidth, newHeight);
}

void AFRenderer::Cleanup()
{
	m_framebufferMS.Cleanup();
	m_framebufferIdPicker.Cleanup();
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
	const glm::vec2& frameBufferSize = m_framebufferMS.GetSize();
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
	m_framebufferMS.Bind();

	// Set the background colour.
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	// Clear colour, depth and stencil buffers.
	glStencilMask(0xFF);      // Enable writing to all bits of the stencil buffer.
	glClearStencil(0);        // Set the clear value for the stencil to 0.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Create the view matrix given camera's transform.
	m_viewMatrix = cameraComp->GetViewMatrix();

	// Inverse view & projection upload.
	m_uniformBuffer.UploadInverseViewProjection(glm::inverse(m_viewMatrix), glm::inverse(m_projectionMatrix));

	// Render properties upload.
	glm::mat4 renderPropertiesMat = AFUtility::CreateRenderPropertiesMat(
		frameBufferSize.x,
		frameBufferSize.y,
		near,
		far,
		cameraMov->GetControlRotation().x);
	m_uniformBuffer.UploadRenderProperties(renderPropertiesMat);

	// View & projection upload.
	m_uniformBuffer.UploadViewProjection(m_viewMatrix, m_projectionMatrix);

	// View rotation upload.
	m_viewMatrix[3] = glm::vec4(0, 0, 0, 1); // Simply get rid of translation.
	m_uniformBuffer.UploadViewRotation(m_viewMatrix);

	// Ortho projection upload.
	m_orthoMatrix = AFUtility::CreateOrthoProjectionMat(frameBufferSize.x, frameBufferSize.y);
	m_uniformBuffer.UploadOrthoProjection(m_orthoMatrix);

	// Camera transform upload.
	m_uniformBuffer.UploadCamera(cameraComp->GetWorldTransform());

	// Per actor draw.
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
	m_framebufferMS.UnBind();

	// Draw all the contents of the frame buffer to the screen.
	m_framebufferMS.DrawToScreen(sceneData);

	// Draw UIs on top of everything.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Per UI draw.
	for (const AFUI* const ui : sceneData.uis)
	{
		// Per component draw.
		for (const AFComponent* const component : ui->GetComponents())
		{
			const AFUIRenderComponent* const uiRenderComponent = dynamic_cast<const AFUIRenderComponent*>(component);
			if (!uiRenderComponent)
			{
				continue;
			}

			// Upload the ui matrix.
			m_uniformBuffer.UploadUITransform(uiRenderComponent->GetUITransform());

			// Draw.
			uiRenderComponent->Draw();
		}
	}

	// Separate draw loop for the id-picker.
	m_framebufferIdPicker.Bind();

	// Set the background colour.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Clear colour, depth and stencil buffers.
	glStencilMask(0xFF);      // Enable writing to all bits of the stencil buffer.
	glClearStencil(0);        // Set the clear value for the stencil to 0.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	AFDrawOverride idPickerDrawProperties;
	idPickerDrawProperties.shader = m_framebufferIdPicker.GetIDPickerShader();

	// Actors.
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

			const IAFPickerInterface* const pickerInterface = dynamic_cast<const IAFPickerInterface*>(component);
			if (!pickerInterface)
			{
				continue;
			}

			// Upload the model matrix.
			m_uniformBuffer.UploadTransform(renderComponent->GetWorldTransform());

			// Draw.
			renderComponent->Draw(true, idPickerDrawProperties);
		}
	}

	// UIs.
	for (const AFUI* const ui : sceneData.uis)
	{
		// Per component draw.
		for (const AFComponent* const component : ui->GetComponents())
		{
			const AFUIRenderComponent* const uiRenderComponent = dynamic_cast<const AFUIRenderComponent*>(component);
			if (!uiRenderComponent)
			{
				continue;
			}

			const IAFPickerInterface* const pickerInterface = dynamic_cast<const IAFPickerInterface*>(component);
			if (!pickerInterface)
			{
				continue;
			}

			// Upload the ui matrix.
			m_uniformBuffer.UploadUITransform(uiRenderComponent->GetUITransform());

			// Draw.
			uiRenderComponent->Draw(true, idPickerDrawProperties);
		}
	}

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	GLubyte pixel[4];
	glReadPixels(0, 0, 1, 1, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &pixel);
	std::cout << "RGBA: " << (int)pixel[0] << ", " << (int)pixel[1] << ", "
		<< (int)pixel[2] << ", " << (int)pixel[3] << std::endl;

	m_framebufferIdPicker.UnBind();
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
