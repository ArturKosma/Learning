#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "AFRenderData.h"
#include "AFFrameBuffer.h"
#include "AFVertexBuffer.h"
#include "AFTexture.h"
#include "AFShader.h"
#include "AFUniformBuffer.h"
#include "AFUserInterface.h"

class AFRenderer
{
	friend class AFWindow;

public:

	bool Init(int width, int height, GLFWwindow* window);

	void SetSize(int newWidth, int newHeight);
	void UploadData(const AFMesh& newMesh);
	void Cleanup();

	static void CheckGLErrors(const char* label);

	const GLubyte* GetOpenGLVersion();

private:

	AFRenderer();
	~AFRenderer();

	void Draw();

	AFShader m_basicShader = AFShader();
	AFShader m_changedShader = AFShader();

	AFFramebuffer m_framebuffer = AFFramebuffer();
	AFVertexBuffer m_vertexBuffer = AFVertexBuffer();
	AFTexture m_tex = AFTexture();
	AFUniformBuffer m_uniformBuffer = AFUniformBuffer();

	AFRenderData m_renderData = AFRenderData();

	AFUserInferface m_userInterface = AFUserInferface();

	glm::mat4 m_viewMatrix = glm::mat4(1.0f);
	glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

	int m_triangleCount = 0;
};
