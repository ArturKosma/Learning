#include "AFUniformBuffer.h"

#include <glm/gtc/type_ptr.hpp>

void AFUniformBuffer::Init()
{
	glGenBuffers(1, &m_uboBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

	// Bind the buffer to the binding point (e.g., 0).
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_uboBuffer);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadUBOData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uboBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboBuffer, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_uboBuffer);
}
