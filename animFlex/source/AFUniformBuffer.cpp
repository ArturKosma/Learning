#include "AFUniformBuffer.h"

#include <glm/gtc/type_ptr.hpp>

void AFUniformBuffer::Init()
{
	// Matrices buffer.

	glGenBuffers(1, &m_matricesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_matricesBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matricesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Camera buffer.

	glGenBuffers(1, &m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_matricesBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(modelMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadCamera(glm::mat4 cameraTrs)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(cameraTrs));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_matricesBuffer);
	glDeleteBuffers(1, &m_cameraBuffer);
}
