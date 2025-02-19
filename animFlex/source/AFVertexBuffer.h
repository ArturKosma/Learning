#pragma once

#include <glad/glad.h> 
#include "AFStructs.h"

class AFVertexBuffer
{
	friend class AFRenderer;

public:

	void Init();
	void UploadMesh(const AFMesh& newMesh);

	void Bind() const;
	void UnBind() const;

	void Draw(GLuint mode, size_t num) const;
	void Cleanup();

private:

	GLuint m_VAO = 0;
	GLuint m_vertexVBO = 0;
	GLuint m_indexEBO = 0;
};	