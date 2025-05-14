#include "AFRenderer.h"

#include "AFCamera.h"
#include "AFMath.h"
#include "AFRenderComponent.h"
#include "AFUIRenderComponent.h"
#include "AFUtility.h"
#include "AFUI.h"

#include <ostream>
#include <glm/ext/matrix_clip_space.hpp>

#include "AFSkeletalMeshComponent.h"
#include "AFTextRender.h"
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

	if (!m_framebufferIdPicker.Init(width, height))
	{
		printf("%s\n", "Fail on frame buffer id picker Init().");
		return false;
	}

	if (!m_framebufferMSUI.Init(width, height))
	{
		printf("%s\n", "Fail on frame buffer ui Init().");
		return false;
	}

	// Init the text rendering.
	if(!AFTextRender::GetInstance().Init())
	{
		printf("%s\n", "Fail on text rendering Init().");
		return false;
	}

	// Set up the uniform buffer.
	m_uniformBuffer.Init();

	// Make sure closer objects are drawn before farther.
	glDepthFunc(GL_LESS);

	// By default enable cull face.
	glEnable(GL_CULL_FACE);

	return true;
}

void AFRenderer::SetSize(int newWidth, int newHeight)
{
	m_framebufferMS.Resize(newWidth, newHeight);
	m_framebufferIdPicker.Resize(newWidth, newHeight);
	m_framebufferMSUI.Resize(newWidth, newHeight);

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

void AFRenderer::Draw(const FAFSceneData& sceneData, const FAFAppData& appData)
{
	std::shared_ptr<AFCamera> camera = sceneData.activeCamera;
	if(!camera)
	{
		return;
	}

	std::shared_ptr<AFCameraComponent> cameraComp = camera->GetCameraComponent();
	if(!cameraComp)
	{
		return;
	}

	std::shared_ptr<AFCameraMovementComponent> cameraMov = camera->GetMovementComponent();
	if(!cameraMov)
	{
		return;
	}

	// Fetch framebuffer size and camera properties.
	const glm::vec2& frameBufferSize = m_framebufferMS.GetSize();
	const FAFCameraProperties& cameraProps = cameraComp->GetCameraProperties();
	const float fov = static_cast<float>(cameraProps.fieldOfView);
	const float near = cameraProps.near;
	const float far = cameraProps.far;

	// Safely create the projection matrix given screen width/height.
	if (frameBufferSize.x > 0 && frameBufferSize.y > 0)
	{
		m_projectionMatrix = glm::perspective(glm::radians(fov), frameBufferSize.x / frameBufferSize.y, near, far);
	}

	// Enable drawing to the main frame buffer.
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
	for(std::shared_ptr<AFActor> sceneActor : sceneData.sceneActors)
	{
		// Per component draw.
		for(std::shared_ptr<AFComponent> component : sceneActor->GetComponents())
		{
			std::shared_ptr<AFRenderComponent> renderComponent = std::dynamic_pointer_cast<AFRenderComponent>(component);
			if(!renderComponent)
			{
				continue;
			}

			// Upload the model matrix.
			m_uniformBuffer.UploadTransform(renderComponent->GetWorldTransform());

			// Upload the joints matrices.
			std::shared_ptr<AFSkeletalMeshComponent> skeletalMeshComponent = std::dynamic_pointer_cast<AFSkeletalMeshComponent>(component);
			if(skeletalMeshComponent)
			{
				m_uniformBuffer.UploadJointsMatrices(skeletalMeshComponent->GetJointsMatrices());
			}

			// Draw.
			renderComponent->Draw();
		}
	}

	// Unbind the frame buffer, we don't want to draw to it anymore.
	m_framebufferMS.UnBind();

	// Draw all the contents of the main frame buffer to the screen.
	if (appData.drawType == EAFDrawType::Normal)
	{
		m_framebufferMS.DrawToScreen(sceneData);
	}

	// Separate draw loop for the ui.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Per UI draw.
	for (std::shared_ptr<AFUI> ui : sceneData.uis)
	{
		// Per component draw.
		for (std::shared_ptr<AFComponent> component : ui->GetComponents())
		{
			std::shared_ptr<AFUIRenderComponent> uiRenderComponent = std::dynamic_pointer_cast<AFUIRenderComponent>(component);
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

	// Clear colour, depth and stencil buffers.
	glStencilMask(0xFF);      // Enable writing to all bits of the stencil buffer.
	glClearStencil(0);        // Set the clear value for the stencil to 0.
	const GLuint clearColor[4] = { 255, 255, 255, 255 };
	glClearBufferuiv(GL_COLOR, 0, clearColor);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Set the background colour.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	FAFDrawOverride idPickerDrawProperties;
	idPickerDrawProperties.drawType = EAFDrawType::IDPicker;
	idPickerDrawProperties.shader = m_framebufferIdPicker.GetIDPickerShader();

	// Actors.
	for (std::shared_ptr<AFActor> sceneActor : sceneData.sceneActors)
	{
		// Per component draw.
		for (std::shared_ptr<AFComponent> component : sceneActor->GetComponents())
		{
			std::shared_ptr<AFRenderComponent> renderComponent = std::dynamic_pointer_cast<AFRenderComponent>(component);
			if (!renderComponent)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);
			if (!pickerInterface)
			{
				continue;
			}

			// Upload the model matrix.
			m_uniformBuffer.UploadTransform(renderComponent->GetWorldTransform());

			// Send unique ID to the object's shader. It will be used as a color.
			idPickerDrawProperties.shader->Use();
			GLuint objectIDLoc = glGetUniformLocation(idPickerDrawProperties.shader->GetProgram(), "uObjectID");
			const glm::u8vec2 objUniqueID = AFUtility::PackID(renderComponent->GetUniqueID());
			glUniform2ui(objectIDLoc, objUniqueID.r, objUniqueID.g);

			// Draw.
			renderComponent->Draw(true, idPickerDrawProperties);
		}
	}

	idPickerDrawProperties.shader = m_framebufferIdPicker.GetIDPickerShader(true);

	// UIs.
	for (std::shared_ptr<AFUI> ui : sceneData.uis)
	{
		// Per component draw.
		for (std::shared_ptr<AFComponent> component : ui->GetComponents())
		{
			std::shared_ptr<AFUIRenderComponent> uiRenderComponent = std::dynamic_pointer_cast<AFUIRenderComponent>(component);
			if (!uiRenderComponent)
			{
				continue;
			}

			std::shared_ptr<IAFPickerInterface> pickerInterface = std::dynamic_pointer_cast<IAFPickerInterface>(component);
			if (!pickerInterface)
			{
				continue;
			}

			// Upload the ui matrix.
			m_uniformBuffer.UploadUITransform(uiRenderComponent->GetUITransform());

			// Send unique ID to the object's shader. It will be used as a color.
			idPickerDrawProperties.shader->Use();
			GLuint objectIDLoc = glGetUniformLocation(idPickerDrawProperties.shader->GetProgram(), "uObjectID");
			const glm::u8vec2 objUniqueID = AFUtility::PackID(uiRenderComponent->GetUniqueID());
			glUniform2ui(objectIDLoc, objUniqueID.r, objUniqueID.g);

			// Draw.
			uiRenderComponent->Draw(idPickerDrawProperties);
		}
	}

	m_framebufferIdPicker.UnBind();

	// ID picker draw.
	if(appData.drawType == EAFDrawType::IDPicker)
	{
		m_framebufferIdPicker.DrawToScreen(sceneData);
	}
}

FAFPickID AFRenderer::ReadColorId(int x, int y)
{
	m_framebufferIdPicker.Bind();

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	GLubyte pixel[4];
	glReadPixels(x, static_cast<int>(glm::abs(y - m_framebufferMS.GetSize().y)), 1, 1, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, &pixel);

	m_framebufferIdPicker.UnBind();

	return AFUtility::UnpackID(pixel);
}

AFRenderer::AFRenderer()
{

}

AFRenderer::~AFRenderer()
{
}
