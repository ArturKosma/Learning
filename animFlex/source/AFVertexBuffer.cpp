#include "AFVertexBuffer.h"
#include "AFStructs.h"

void AFVertexBuffer::Init(EAFVertexBufferType vertexBufferType)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_vertexVBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

	switch(vertexBufferType)
	{
		case EAFVertexBufferType::StaticMesh:
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, position));
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, color));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, uv));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			break;
		}
		case EAFVertexBufferType::Grid:
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AFVertex), (void*)offsetof(AFVertex, position));

			glEnableVertexAttribArray(0);

			break;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AFVertexBuffer::UploadMesh(const AFMesh& newMesh)
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexVBO);

	glBufferData(GL_ARRAY_BUFFER, newMesh.vertices.size() * sizeof(AFVertex), &newMesh.vertices.at(0), GL_DYNAMIC_DRAW);

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

void AFVertexBuffer::Draw(GLuint mode, unsigned int start, size_t num) const
{
	glDrawArrays(mode, start, static_cast<GLsizei>(num));
}

void AFVertexBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_vertexVBO);
	glDeleteVertexArrays(1, &m_VAO);
}
