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

	// Inverse View & Projection buffer.
	glGenBuffers(1, &m_inverseViewProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_inverseViewProjectionBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_inverseViewProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Transform buffer.
	glGenBuffers(1, &m_transformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_transformBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_transformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Camera buffer.
	glGenBuffers(1, &m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_cameraBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 3, m_cameraBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Various render properties buffer.
	glGenBuffers(1, &m_renderPropertiesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_renderPropertiesBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 4, m_renderPropertiesBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// UI transform buffer.
	glGenBuffers(1, &m_uiTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_uiTransformBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 5, m_uiTransformBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// View rotation buffer.
	glGenBuffers(1, &m_viewRotationBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_viewRotationBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 6, m_viewRotationBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Ortho projection buffer.
	glGenBuffers(1, &m_orthoProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, m_orthoProjectionBuffer);

	glBufferData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);

	glBindBufferBase(GL_UNIFORM_BUFFER, 7, m_orthoProjectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadViewProjection(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_viewProjectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadViewRotation(glm::mat4 viewRotationMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_viewRotationBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewRotationMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadOrthoProjection(glm::mat4 orthoProjectionMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_orthoProjectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(orthoProjectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadInverseViewProjection(glm::mat4 inverseViewMatrix, glm::mat4 inverseProjectionMatrix)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_inverseViewProjectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(inverseViewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(inverseProjectionMatrix));
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

void AFUniformBuffer::UploadRenderProperties(glm::mat4 renderProperties)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_renderPropertiesBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(renderProperties));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::UploadUITransform(glm::mat4 uiTransform)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_uiTransformBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(uiTransform));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AFUniformBuffer::Cleanup()
{
	glDeleteBuffers(1, &m_viewProjectionBuffer);
	glDeleteBuffers(1, &m_inverseViewProjectionBuffer);
	glDeleteBuffers(1, &m_transformBuffer);
	glDeleteBuffers(1, &m_cameraBuffer);
	glDeleteBuffers(1, &m_renderPropertiesBuffer);
	glDeleteBuffers(1, &m_uiTransformBuffer);
	glDeleteBuffers(1, &m_viewRotationBuffer);
	glDeleteBuffers(1, &m_orthoProjectionBuffer);
}
