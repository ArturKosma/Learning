#include "AFFrameBuffer.h"

#include "AFCamera.h"
#include "AFStructs.h"

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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGBA16F, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_msColorBuffer);

	// Create multisampled depth renderbuffer.
	glGenRenderbuffers(1, &m_msDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_msDepthBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_msDepthBuffer);

	// Check if multisampling FBO is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create resolve framebuffer.
	glGenFramebuffers(1, &m_resolveFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_resolveFramebuffer);

	// Create resolve color texture.
	glGenTextures(1, &m_resolveColorTex);
	glBindTexture(GL_TEXTURE_2D, m_resolveColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_resolveColorTex, 0);

	// Create resolve depth texture.
	glGenTextures(1, &m_resolveDepthTex);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_resolveDepthTex, 0);

	// Check if resolve FBO is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

	// Bind the multisampled framebuffer as read.
	// Bind the resolve framebuffer as draw.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveFramebuffer);

	// Blit color & depth to a resolve texture.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight,
	GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	// Disable depth testing before drawing screen quad.
	glDisable(GL_DEPTH_TEST);

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Bind the resolved textures to texture units.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_resolveColorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);

	// Set use postprocess shaders.
	// Set resolution uniform.
	AFPostprocessShader cameraPostprocess = cameraComp->GetPostprocessShader();
	cameraPostprocess.Use();
	cameraPostprocess.SetUniform2f("u_Resolution", GetSize().x, GetSize().y);

	// Draw 3 undefined points.
	// Postprocess vertex shader should create a full screen triangle.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AFFramebuffer::Cleanup()
{
	UnBind();
	Delete();
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
	glDeleteTextures(1, &m_resolveColorTex);
	glDeleteRenderbuffers(1, &m_msColorBuffer);
	glDeleteRenderbuffers(1, &m_msDepthBuffer);
	glDeleteFramebuffers(1, &m_msFBO);
	glDeleteFramebuffers(1, &m_resolveFramebuffer);
}
