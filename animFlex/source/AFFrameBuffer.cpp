#include "AFFrameBuffer.h"

#include <iostream>
#include <ostream>

#include "AFStructs.h"
#include "AFUtility.h"

bool AFFramebuffer::Init(int width, int height)
{
	// Create resolve framebuffer.
	glGenFramebuffers(1, &m_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

	// Create single-sample texture for resolve color output.
	glGenTextures(1, &m_colorTex);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTex, 0);

	// Create single-sample depth renderbuffer.
	glGenRenderbuffers(1, &m_depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create quad for the screen.
	AFMesh screenMesh;
	screenMesh.vertices.resize(6);
	screenMesh.vertices[0].position = glm::vec3(-0.8f, 0.8f, 0.0f); // Top left.
	screenMesh.vertices[1].position = glm::vec3(-1.0f, -1.0f, 0.0f); // Lower left.
	screenMesh.vertices[2].position = glm::vec3(1.0f, -1.0f, 0.0f); // Lower right.
	screenMesh.vertices[3].position = glm::vec3(-0.8f, 0.8f, 0.0f); // Top left.
	screenMesh.vertices[4].position = glm::vec3(1.0f, -1.0f, 0.0f); // Lower right.
	screenMesh.vertices[5].position = glm::vec3(1.0f, 1.0f, 0.0f); // Upper right.
	screenMesh.vertices[0].color = glm::vec3(0.0f, 0.0f, 0.0f);
	screenMesh.vertices[1].color = glm::vec3(0.0f, 0.0f, 0.0f);
	screenMesh.vertices[2].color = glm::vec3(1.0f, 1.0f, 1.0f);
	screenMesh.vertices[3].color = glm::vec3(0.0f, 0.0f, 0.0f);
	screenMesh.vertices[4].color = glm::vec3(1.0f, 1.0f, 1.0f);
	screenMesh.vertices[5].color = glm::vec3(1.0f, 1.0f, 1.0f);
	screenMesh.vertices[0].uv = glm::vec2(0.0f, 1.0f);
	screenMesh.vertices[1].uv = glm::vec2(0.0f, 0.0f);
	screenMesh.vertices[2].uv = glm::vec2(1.0f, 0.0f);
	screenMesh.vertices[3].uv = glm::vec2(0.0f, 0.0f);
	screenMesh.vertices[4].uv = glm::vec2(1.0f, 0.0f);
	screenMesh.vertices[5].uv = glm::vec2(1.0f, 1.0f);

	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVBO);

	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);
	glBufferData(GL_ARRAY_BUFFER, screenMesh.vertices.size() * sizeof(AFVertex), &screenMesh.vertices.at(0), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, uv));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	m_screenShader.SetVertexShader("content/shaders/screen.vert");
	m_screenShader.SetFragmentShader("content/shaders/screen.frag");
	const bool shadersLoaded = m_screenShader.LoadShaders();

	return CheckComplete();
}

bool AFFramebuffer::TESTInit(int width, int height)
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
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, width, height); // 4x MSAA.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_msDepthBuffer);

	// Check if multisampling FBO is complete.
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool AFFramebuffer::Resize(int newWidth, int newHeight)
{
	m_bufferWidth = newWidth;
	m_bufferHeight = newHeight;

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glDeleteTextures(1, &m_colorTex);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteFramebuffers(1, &m_buffer);

	return Init(newWidth, newHeight);
}

glm::vec2 AFFramebuffer::GetSize() const
{
	return { m_bufferWidth, m_bufferHeight };
}

void AFFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
}

void AFFramebuffer::TESTBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_msFBO);
}

void AFFramebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AFFramebuffer::DrawToScreen()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_buffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
}

void AFFramebuffer::TESTDrawToScreen()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight,
		0, 0, m_bufferWidth, m_bufferHeight,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);

	/*
	// Unbind framebuffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Bind the default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Use the screen shader
	m_screenShader.Use();

	// Bind the resolved texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);

	// Set shader uniform
	GLint texLocation = glGetUniformLocation(m_screenShader.GetProgram(), "Tex");
	glUniform1i(texLocation, 0);

	// Draw the fullscreen quad
	glBindVertexArray(m_screenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	*/
	

	// WORKS AS RED SCREEN!

	/*
	// Bind the default framebuffer (screen)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Use the screen shader
	m_screenShader.Use();

	// Bind the texture from `m_buffer`
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_colorTex);

	// Set uniform to tell shader to sample from texture unit 0
	GLint texLocation = glGetUniformLocation(m_screenShader.GetProgram(), "Tex");
	glUniform1i(texLocation, 0);

	// Draw a fullscreen quad
	glBindVertexArray(m_screenVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
	*/

	DebugOpenGL();
}

void AFFramebuffer::Cleanup()
{
	UnBind();
	glDeleteTextures(1, &m_colorTex);
	glDeleteRenderbuffers(1, &m_depthBuffer);
	glDeleteFramebuffers(1, &m_buffer);
}

AFFramebuffer::AFFramebuffer()
{
}

AFFramebuffer::~AFFramebuffer()
{
}

bool AFFramebuffer::CheckComplete() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

	const GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(result != GL_FRAMEBUFFER_COMPLETE)
	{
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
