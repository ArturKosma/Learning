#pragma once

#include "AFRenderData.h"
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class AFVertexBuffer
{
	friend class AFRenderer;

public:

	void Init();

	void UploadData(const AFMesh& vertexData);

	void Bind();
	void UnBind();

	void Draw(GLuint mode, unsigned int start, unsigned int num);
	void Cleanup();

private:

	AFVertexBuffer();
	~AFVertexBuffer();

	GLuint m_VAO = 0;
	GLuint m_vertexVBO = 0;
};	