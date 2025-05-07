#pragma once

#include <glad/glad.h> 
#include "AFStructs.h"

class AFVertexBuffer
{
	friend class AFRenderer;

public:

	void Init();
	void UploadMesh(const AFMesh& newMesh);
	void UploadMesh(const AFMeshLoaded& newMesh);

	void Bind() const;
	void UnBind() const;

	void Draw(GLuint mode) const;
	void Cleanup();

private:

	GLsizei m_indexCount = 0;
	GLenum m_type = GL_UNSIGNED_INT;

	GLuint m_VAO = 0;
	GLuint m_vertexVBO = 0;
	GLuint m_indexEBO = 0;
};	