#include "AFVertexBuffer.h"

void AFVertexBuffer::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_vertexVBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*) offsetof(AFVertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*) offsetof(AFVertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*) offsetof(AFVertex, uv));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AFVertexBuffer::UploadData(const AFMesh& vertexData)
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

	glBufferData(GL_ARRAY_BUFFER, vertexData.vertices.size() * sizeof(AFVertex), &vertexData.vertices.at(0), GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
}

void AFVertexBuffer::Bind()
{
	glBindVertexArray(m_VAO);
}

void AFVertexBuffer::UnBind()
{
	glBindVertexArray(0);
}

void AFVertexBuffer::Draw(GLuint mode, unsigned int start, unsigned int num)
{
	glDrawArrays(mode, start, num);
}

void AFVertexBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteVertexArrays(1, &m_VAO);
}

AFVertexBuffer::AFVertexBuffer()
{
}

AFVertexBuffer::~AFVertexBuffer()
{
}
