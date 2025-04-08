#include "AFFrameBuffer.h"

#include "AFCamera.h"
#include "AFStructs.h"
#include "AFUtility.h"

bool AFFramebuffer::Init(int width, int height)
{
	m_bufferWidth = width;
	m_bufferHeight = height;

	// Create multisampled framebuffer.
	glGenFramebuffers(1, &m_msFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_msFBO);

	// Create multisampled color renderbuffer.
	glGenRenderbuffers(1, &m_msColorBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_msColorBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA8, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_msColorBuffer);

	// Create multisampled depth renderbuffer.
	glGenRenderbuffers(1, &m_msDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_msDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_msDepthBuffer);

	// Check if multisampling FBO is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create resolve framebuffer 0.
	glGenFramebuffers(1, &m_resolveFramebuffer0);
	glBindFramebuffer(GL_FRAMEBUFFER, m_resolveFramebuffer0);

	// Create resolve color texture 0.
	glGenTextures(1, &m_resolveColorTex0);
	glBindTexture(GL_TEXTURE_2D, m_resolveColorTex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolveColorTex0, 0);

	// Check if resolve FBO 0 is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create resolve framebuffer 1.
	glGenFramebuffers(1, &m_resolveFramebuffer1);
	glBindFramebuffer(GL_FRAMEBUFFER, m_resolveFramebuffer1);

	// Create resolve color texture 1.
	glGenTextures(1, &m_resolveColorTex1);
	glBindTexture(GL_TEXTURE_2D, m_resolveColorTex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolveColorTex1, 0);

	// Check if resolve FBO 1 is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create resolve depth texture.
	glGenTextures(1, &m_resolveDepthTex);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_resolveDepthTex, 0);

	// Create depth resolve FBO.
	glGenFramebuffers(1, &m_depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_resolveDepthTex, 0);

	// Check if depth resolve FBO is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create resolve stencil color texture.
	glGenTextures(1, &m_resolveStencilColorTex);
	glBindTexture(GL_TEXTURE_2D, m_resolveStencilColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create resolve actual stencil texture.
	glGenTextures(1, &m_resolveStencilTex);
	glBindTexture(GL_TEXTURE_2D, m_resolveStencilTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create stencil resolve FBO.
	glGenFramebuffers(1, &m_stencilFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_stencilFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolveStencilColorTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_resolveStencilTex, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_stencilShader.SetVertexShader("content/shaders/stencil.vert");
	m_stencilShader.SetFragmentShader("content/shaders/stencil.frag");
	m_stencilShader.LoadShaders();

	return true;
}

bool AFFramebuffer::Resize(int newWidth, int newHeight)
{
	m_bufferWidth = newWidth;
	m_bufferHeight = newHeight;

	Delete();

	return Init(newWidth, newHeight);
}

glm::vec2 AFFramebuffer::GetSize() const
{
	return { m_bufferWidth, m_bufferHeight };
}

void AFFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_msFBO);
}

void AFFramebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AFFramebuffer::DrawToScreen(const AFSceneData& sceneData)
{
	AFCamera* camera = sceneData.activeCamera;
	if (!camera)
	{
		return;
	}

	AFCameraComponent* cameraComp = camera->GetCameraComponent();
	if (!cameraComp)
	{
		return;
	}

	// Clear the stencil texture.
	ClearStencil();

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
	m_stencilShader.Use();
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Disable stencil test.
	glDisable(GL_STENCIL_TEST);

	// Disable framebuffer bindings.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Fetch all the camera postprocesses.
	std::vector<AFPostprocessShader> cameraPostprocessShaders = cameraComp->GetPostprocessShaders();

	// Alternate between resolve FBOs while applying all postprocess shaders.
	std::vector<GLuint> resolveFbo = { m_resolveFramebuffer0, m_resolveFramebuffer1 };
	std::vector<GLuint> resolveColorTex = { m_resolveColorTex0, m_resolveColorTex1 };
	GLuint finalColorTex = m_resolveColorTex0;
	bool alternate = false;
	for(AFPostprocessShader postprocess : cameraPostprocessShaders)
	{
		const GLuint drawIdx = alternate ? 0 : 1;

		// Bind the target framebuffer.
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolveFbo[drawIdx]);

		// Bind the final textures.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, finalColorTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_resolveStencilColorTex);

		// Use postprocess.
		postprocess.Use();

		// Draw 3 undefined points.
		// Postprocess vertex shader should create a full screen triangle.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		finalColorTex = resolveColorTex[drawIdx];

		// Alternate.
		alternate = !alternate;
	}

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use neutral shader.
	cameraComp->GetNeutralShader().Use();

	// Bind the final textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalColorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_resolveStencilColorTex);

	// Draw 3 undefined points.
	// Final vertex shader should create a full screen triangle.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AFFramebuffer::Cleanup()
{
	UnBind();
	Delete();
}

void AFFramebuffer::ClearStencil()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_stencilFBO);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

AFFramebuffer::AFFramebuffer()
{
}

AFFramebuffer::~AFFramebuffer()
{
}

void AFFramebuffer::Delete()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDeleteTextures(1, &m_resolveDepthTex);
	glDeleteTextures(1, &m_resolveColorTex0);
	glDeleteTextures(1, &m_resolveColorTex1);
	glDeleteTextures(1, &m_resolveStencilTex);
	glDeleteTextures(1, &m_resolveStencilColorTex);
	glDeleteRenderbuffers(1, &m_msColorBuffer);
	glDeleteRenderbuffers(1, &m_msDepthBuffer);
	glDeleteFramebuffers(1, &m_msFBO);
	glDeleteFramebuffers(1, &m_resolveFramebuffer0);
	glDeleteFramebuffers(1, &m_resolveFramebuffer1);
	glDeleteFramebuffers(1, &m_stencilFBO);
	glDeleteFramebuffers(1, &m_depthFBO);
}
