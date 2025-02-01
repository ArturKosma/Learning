#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "AFScreenShader.h"

class AFFramebuffer
{
	friend class AFRenderer;

public:

	bool Init(int width, int height);
	bool TESTInit(int width, int height);
	bool Resize(int newWidth, int newHeight);
	glm::vec2 GetSize() const;

	void Bind();
	void TESTBind();
	void UnBind();

	void DrawToScreen();
	void TESTDrawToScreen();
	void Cleanup();

private:

	AFFramebuffer();
	~AFFramebuffer();

	bool CheckComplete() const;

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_msFBO = 0;
	GLuint m_msColorBuffer = 0;
	GLuint m_msDepthBuffer = 0;

	GLuint m_buffer = 0;
	GLuint m_colorTex = 0;
	GLuint m_depthBuffer = 0;

	GLuint m_screenVAO = 0;
	GLuint m_screenVBO = 0;
};
