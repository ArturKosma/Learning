#include "AFFrameBuffer.h"

#include <ostream>

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
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

	// Init postprocess shader.
	m_postprocessShader.SetVertexShader("content/shaders/postprocess.vert");
	m_postprocessShader.SetFragmentShader("content/shaders/postprocess.frag");
	m_postprocessShader.LoadShaders();

	// Create VAO & VBO for the screen quad.
	glGenVertexArrays(1, &m_screenVAO);
	glGenBuffers(1, &m_screenVBO);
	glBindVertexArray(m_screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_screenVBO);

	m_screenMesh.vertices.clear();
	m_screenMesh.vertices.resize(6);
	m_screenMesh.vertices[0] = { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	m_screenMesh.vertices[1] = { glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f) };
	m_screenMesh.vertices[2] = { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	m_screenMesh.vertices[3] = { glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f) };
	m_screenMesh.vertices[4] = { glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f) };
	m_screenMesh.vertices[5] = { glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f) };

	glBufferData(GL_ARRAY_BUFFER, m_screenMesh.vertices.size() * sizeof(AFVertex), &m_screenMesh.vertices.at(0), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, color));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, uv));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
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

void AFFramebuffer::SetZNearFar(float zNear, float zFar)
{
	m_zNear = zNear;
	m_zFar = zFar;
}

void AFFramebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_msFBO);
}

void AFFramebuffer::UnBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void AFFramebuffer::DrawToScreen()
{
	// Disable depth testing before drawing screen quad.
	glDisable(GL_DEPTH_TEST);

	// Bind the multisampled framebuffer as read.
	// Bind the resolve framebuffer as draw.
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_msFBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_resolveFramebuffer);

	// Blit color & depth.
	glBlitFramebuffer(0, 0, m_bufferWidth, m_bufferHeight, 0, 0, m_bufferWidth, m_bufferHeight, 
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

	// Start drawing to default framebuffer.
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	// Bind the resolved textures to texture units.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_resolveColorTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_resolveDepthTex);

	// Set use postprocess shaders.
	m_postprocessShader.Use();

	GLint zNearLoc = glGetUniformLocation(m_postprocessShader.GetProgram(), "u_zNear");
	GLint zFarLoc = glGetUniformLocation(m_postprocessShader.GetProgram(), "u_zFar");

	// Push uniform zNear and zFar.
	if (zNearLoc != -1)
	{
		glUniform1f(zNearLoc, m_zNear);
	}
	if (zFarLoc != -1)
	{
		glUniform1f(zFarLoc, m_zFar);
	}

	// Bind screen quad VAO.
	glBindVertexArray(m_screenVAO);

	// Draw arrays.
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// Unbind screen quad VAO.
	glBindVertexArray(0);

	// Unbind textures.
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
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
