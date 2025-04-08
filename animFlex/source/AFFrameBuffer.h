#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>

#include "AFShader.h"
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

	void ClearStencil();

private:

	AFFramebuffer();
	~AFFramebuffer();

	void Delete();

	GLint m_bufferWidth = 800;
	GLint m_bufferHeight = 600;

	GLuint m_resolveFramebuffer0 = 0;
	GLuint m_resolveFramebuffer1 = 0;
	GLuint m_resolveColorTex0 = 0;
	GLuint m_resolveColorTex1 = 0;
	GLuint m_resolveDepthTex = 0;

	GLuint m_resolveStencilColorTex = 0;
	GLuint m_resolveStencilTex = 0;

	GLuint m_msFBO = 0;
	GLuint m_msColorBuffer = 0;
	GLuint m_msDepthBuffer = 0;

	GLuint m_depthFBO = 0;
	GLuint m_stencilFBO = 0;

	AFShader m_stencilShader = AFShader();
};
