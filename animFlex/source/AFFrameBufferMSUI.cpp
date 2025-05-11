#include "AFFrameBufferMSUI.h"

#include "AFCamera.h"
#include "AFCameraComponent.h"

void AFFramebufferMSUI::Bind()
{
	AFFramebuffer::Bind();
}

void AFFramebufferMSUI::UnBind()
{
	glDisable(GL_BLEND);
	AFFramebuffer::UnBind();
}

void AFFramebufferMSUI::DrawToScreen(const FAFSceneData& sceneData)
{
	std::shared_ptr<AFCamera> camera = sceneData.activeCamera;
	if (!camera)
	{
		return;
	}

	std::shared_ptr<AFCameraComponent> cameraComp = camera->GetCameraComponent();
	if (!cameraComp)
	{
		return;
	}

	// Clear the stencil texture.
	glBindFramebuffer(GL_FRAMEBUFFER, m_stencilFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Bind the multisampled framebuffer as read.
	// Bind the resolve0 framebuffer as draw.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveFramebuffer0);

	// Blit color to a resolve texture.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// Bind the multisampled framebuffer as read.
	// Bind the depth resolve framebuffer as draw.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_depthFBO);

	// Blit depth to a resolve texture.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight,
		GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// Disable depth testing before drawing any full screen triangle.
	glDisable(GL_DEPTH_TEST);

	// Create stencil color texture m_resolveStencilColorTex.

	// Bind the multisampled framebuffer as read.
	// Bind stencil FBO as draw, to blit stencil data into it.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_stencilFBO);

	// Blit depth & stencil to a stencil texture.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight,
		GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	// Fill a stencil framebuffer.
	// We will use it as a texture in shaders.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_stencilFBO);

	// Settings for the stencil.
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF); // Render only where stencil != 0.
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	// Draw 3 undefined points.
	// Postprocess vertex shader should create a full screen triangle.
	// Stencil texture gets filled.
	m_stencilShader->Use();
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Disable stencil test.
	glDisable(GL_STENCIL_TEST);

	// Disable framebuffer bindings.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Fetch all the camera postprocesses.
	std::vector<std::shared_ptr<AFPostprocessShader>> cameraPostprocessShaders = cameraComp->GetPostprocessShaders();

	GLuint finalColorTex = m_resolveColorTex0;

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glClear(GL_COLOR_BUFFER_BIT);

	// Use ui fullscreen shader.
	cameraComp->GetUIFullScreenShader()->Use();

	// Bind the final textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalColorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_resolveStencilColorTex);

	// Enable alpha blending for UI compositing.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Draw 3 undefined points.
	// Final vertex shader should create a full screen triangle.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisable(GL_BLEND);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}
