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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT24, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_msDepthBuffer);

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

	// Create resolve depth texture 0.
	glGenTextures(1, &m_resolveDepthTex0);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_resolveDepthTex0, 0);

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

	// Create resolve depth texture 1.
	glGenTextures(1, &m_resolveDepthTex1);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_resolveDepthTex1, 0);

	// Check if resolve FBO 1 is complete.
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
	// Bind the resolve0 framebuffer as draw.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveFramebuffer0);

	// Blit color & depth to a resolve texture.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight,
	GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);

	// Disable depth testing before drawing any full screen triangle.
	glDisable(GL_DEPTH_TEST);

	// Fetch all the camera postprocesses.
	std::vector<AFPostprocessShader> cameraPostprocessShaders = cameraComp->GetPostprocessShaders();

	// Alternate between resolve FBOs while applying all postprocess shaders.
	std::vector<GLuint> resolveFbo = { m_resolveFramebuffer0, m_resolveFramebuffer1 };
	std::vector<GLuint> resolveColorTex = { m_resolveColorTex0, m_resolveColorTex1 };
	std::vector<GLuint> resolveDepthTex = { m_resolveDepthTex0, m_resolveDepthTex1 };
	GLuint finalColorTex = m_resolveColorTex0;
	GLuint finalDepthTex = m_resolveDepthTex0;
	bool alternate = false;
	for(AFPostprocessShader postprocess : cameraPostprocessShaders)
	{
		const GLuint readIdx = alternate ? 1 : 0;
		const GLuint drawIdx = alternate ? 0 : 1;

		// Bind read and draw FBOs.
		glBindFramebuffer(GL_READ_FRAMEBUFFER, resolveFbo[readIdx]);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, resolveFbo[drawIdx]);

		// Bind the final textures.
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, finalColorTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, finalDepthTex);

		// Use postprocess.
		postprocess.Use();

		// Draw 3 undefined points.
		// Postprocess vertex shader should create a full screen triangle.
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Copy color and depth to final textures.
		finalColorTex = resolveColorTex[drawIdx];
		finalDepthTex = resolveDepthTex[drawIdx];

		// Alternate.
		alternate = !alternate;
	}

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Bind the final textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, finalColorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, finalDepthTex);

	// Use neutral shader.
	cameraComp->GetNeutralShader().Use();

	// Draw 3 undefined points.
	// Postprocess vertex shader should create a full screen triangle.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	glDeleteTextures(1, &m_resolveDepthTex0);
	glDeleteTextures(1, &m_resolveDepthTex1);
	glDeleteTextures(1, &m_resolveColorTex0);
	glDeleteTextures(1, &m_resolveColorTex1);
	glDeleteRenderbuffers(1, &m_msColorBuffer);
	glDeleteRenderbuffers(1, &m_msDepthBuffer);
	glDeleteFramebuffers(1, &m_msFBO);
	glDeleteFramebuffers(1, &m_resolveFramebuffer0);
	glDeleteFramebuffers(1, &m_resolveFramebuffer1);
}
