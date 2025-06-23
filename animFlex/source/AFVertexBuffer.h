#pragma once

#include <glad/glad.h> 
#include "AFStructs.h"

class AFSubMesh;

class AFVertexBuffer
{
	friend class AFRenderer;

public:

	void Init();

	void UploadMesh(const AFSubMesh& newSubMesh);
	void UploadMesh(const FAFSubMeshLoaded& newSubMesh);

	void SetDrawMode(GLuint newDrawMode);

	void Bind() const;
	void UnBind() const;

	void Draw() const;
	void Cleanup();

private:

	GLuint m_drawMode = GL_TRIANGLES;
	GLsizei m_drawCount = 0;
	GLenum m_drawType = GL_UNSIGNED_INT;

	GLuint m_VAO = 0;
	GLuint m_vertexVBO = 0;
	GLuint m_indexEBO = 0;
};	