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

class AFRenderer
{
	friend class AFWindow;

public:

	bool Init(int width, int height);

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

	int m_triangleCount = 0;
};
