#include "AFVertexBuffer.h"
#include "AFStructs.h"
#include "AFUtility.h"

void AFVertexBuffer::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_vertexVBO);
	glGenBuffers(1, &m_indexEBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, uv));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, jointNum));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, jointWeight));
	glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, faceID));
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(FAFVertex), (void*)offsetof(FAFVertex, uvCenter));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexEBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AFVertexBuffer::UploadMesh(const FAFSubMesh& newSubMesh)
{
	m_drawCount = static_cast<GLsizei>(newSubMesh.indices.size());

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, newSubMesh.vertices.size() * sizeof(FAFVertex), &newSubMesh.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newSubMesh.indices.size() * sizeof(unsigned int), &newSubMesh.indices.at(0), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void AFVertexBuffer::UploadMesh(const FAFSubMeshLoaded& newSubMesh)
{
	m_VAO = newSubMesh.vao;
	m_drawMode = newSubMesh.drawMode;
	m_drawCount = newSubMesh.drawCount;
	m_drawType = newSubMesh.drawType;
}

void AFVertexBuffer::SetDrawMode(GLuint newDrawMode)
{
	m_drawMode = newDrawMode;
}

void AFVertexBuffer::Bind() const
{
	glBindVertexArray(m_VAO);
}

void AFVertexBuffer::UnBind() const
{
	glBindVertexArray(0);
}

void AFVertexBuffer::Draw() const
{
	glDrawElements(m_drawMode, m_drawCount, m_drawType, nullptr);
}

void AFVertexBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteBuffers(1, &m_indexEBO);
	glDeleteVertexArrays(1, &m_VAO);
}
