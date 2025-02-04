#include "AFUniformBuffer.h"

#include <glm/gtc/type_ptr.hpp>

void AFUniformBuffer::Init()
{
	// View & Projection buffer.
	glGenBuffers(1, &m_viewProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_viewProjectionBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_viewProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Transform buffer.
	glGenBuffers(1, &m_transformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_transformBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_transformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Camera buffer.
	glGenBuffers(1, &m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadViewProjection(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_viewProjectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadTransform(glm::mat4 modelMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_transformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(modelMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadCamera(glm::mat4 cameraTrs)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(cameraTrs));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_viewProjectionBuffer);
	glDeleteBuffers(1, &m_transformBuffer);
	glDeleteBuffers(1, &m_cameraBuffer);
}
