#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "AFStructs.h"

class AFFramebuffer
{
	friend class AFRenderer;

public:

	bool Init(int width, int height);
	bool Resize(int newWidth, int newHeight);
	glm::vec2 GetSize() const;

	void Bind();
	void UnBind();

	void DrawToScreen(const AFSceneData& sceneData);
	void Cleanup();

private:

	AFFramebuffer();
	~AFFramebuffer();

	void Delete();

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_resolveFramebuffer = 0;
	GLuint m_resolveColorTex = 0;
	GLuint m_resolveDepthTex = 0;

	GLuint m_msFBO = 0;
	GLuint m_msColorBuffer = 0;
	GLuint m_msDepthBuffer = 0;
};
