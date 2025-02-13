#pragma once 

#include <glm/glm.hpp>
#include <glad/glad.h>

class AFUniformBuffer
{
public:

	void Init();

	void UploadViewProjection(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void UploadInverseViewProjection(glm::mat4 inverseViewMatrix, glm::mat4 inverseProjectionMatrix);
	void UploadTransform(glm::mat4 modelMatrix);
	void UploadCamera(glm::mat4 cameraTrs);
	void UploadRenderProperties(glm::mat4 renderProperties);

	void Cleanup();

private:

	GLuint m_viewProjectionBuffer = 0;
	GLuint m_inverseViewProjectionBuffer = 0;
	GLuint m_transformBuffer = 0;
	GLuint m_cameraBuffer = 0;
	GLuint m_renderPropertiesBuffer = 0;
};
