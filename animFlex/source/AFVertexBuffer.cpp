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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, uv));
	glVertexAttribIPointer(2, 4, GL_UNSIGNED_BYTE, sizeof(AFVertex), (void*)offsetof(AFVertex, uniqueId));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexEBO);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AFVertexBuffer::UploadMesh(const AFMesh& newMesh)
{
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, newMesh.vertices.size() * sizeof(AFVertex), &newMesh.vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newMesh.indices.size() * sizeof(unsigned int), &newMesh.indices.at(0), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void AFVertexBuffer::Bind() const
{
	glBindVertexArray(m_VAO);
}

void AFVertexBuffer::UnBind() const
{
	glBindVertexArray(0);
}

void AFVertexBuffer::Draw(GLuint mode, size_t num) const
{
	glDrawElements(mode, static_cast<GLsizei>(num), GL_UNSIGNED_INT, nullptr);
}

void AFVertexBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteBuffers(1, &m_indexEBO);
	glDeleteVertexArrays(1, &m_VAO);
}
