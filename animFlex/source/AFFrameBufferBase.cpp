#include "AFFrameBufferBase.h"

#include <iostream>
#include <ostream>

#include "AFCamera.h"

bool AFFrameBufferBase::Init(int width, int height)
{
	m_bufferWidth = width;
	m_bufferHeight = height;

	m_idPickerShader.SetVertexShader("content/shaders/idPicker.vert");
	m_idPickerShader.SetFragmentShader("content/shaders/idPicker.frag");
	m_idPickerShader.LoadShaders();

	// Create basic frame buffer.
	glGenFramebuffers(1, &m_basicFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_basicFramebuffer);

	// Create basic RGBA texture.
	glGenTextures(1, &m_basicColorTex);
	glBindTexture(GL_TEXTURE_2D, m_basicColorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8UI, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_basicColorTex, 0);

	// Generate and attach a depth buffer.
	glGenRenderbuffers(1, &m_basicDepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_basicDepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_basicDepthBuffer);

	// Check if basic frame buffer is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void AFFrameBufferBase::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_basicFramebuffer);
}

void AFFrameBufferBase::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool AFFrameBufferBase::Resize(int newWidth, int newHeight)
{
	m_bufferWidth = newWidth;
	m_bufferHeight = newHeight;

	Delete();

	return Init(newWidth, newHeight);
}

glm::vec2 AFFrameBufferBase::GetSize() const
{
	return { m_bufferWidth, m_bufferHeight };
}

void AFFrameBufferBase::DrawToScreen(const AFSceneData& sceneData)
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

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use neutral shader.
	cameraComp->GetNeutralShader().Use();

	// Bind the final textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_basicColorTex);

	// Disable depth testing before drawing any full screen triangle.
	glDisable(GL_DEPTH_TEST);

	// Disable stencil test.
	glDisable(GL_STENCIL_TEST);

	// Draw 3 undefined points.
	// Final vertex shader should create a full screen triangle.
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// Disable framebuffer bindings.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void AFFrameBufferBase::Cleanup()
{
	UnBind();
	Delete();
}

const AFShader& AFFrameBufferBase::GetIDPickerShader() const
{
	return m_idPickerShader;
}

void AFFrameBufferBase::Delete()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

AFFrameBufferBase::~AFFrameBufferBase()
{
}
