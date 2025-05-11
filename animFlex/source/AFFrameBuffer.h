#pragma once

#include "AFFrameBufferBase.h"
#include "AFShader.h"

class AFFramebuffer : public AFFrameBufferBase
{
	friend class AFRenderer;

public:

	bool Init(int width, int height) override;

	void Bind() override;
	void UnBind() override;

	void DrawToScreen(const FAFSceneData& sceneData) override;

protected:

	AFFramebuffer() = default;
	~AFFramebuffer() override;

	void Delete() override;

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

	std::shared_ptr<AFShader> m_stencilShader = nullptr;
};
