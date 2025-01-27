#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "AFFrameBuffer.h"
#include "AFUniformBuffer.h"

class AFRenderer
{
	friend class AFApp;

public:

	bool Init(int width, int height);

	void SetSize(int newWidth, int newHeight);
	void Cleanup();

	static void CheckGLErrors(const char* label);

	const GLubyte* GetOpenGLVersion();

private:

	AFRenderer();
	~AFRenderer();

	void Draw(const struct AFSceneData& sceneData);

	AFFramebuffer m_framebuffer = AFFramebuffer();
	AFUniformBuffer m_uniformBuffer = AFUniformBuffer();

	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
};
