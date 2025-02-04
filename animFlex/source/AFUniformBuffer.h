#pragma once 

#include <glm/glm.hpp>
#include <glad/glad.h>

class AFUniformBuffer
{
public:

	void Init();

	void UploadViewProjection(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void UploadTransform(glm::mat4 modelMatrix);
	void UploadCamera(glm::mat4 cameraTrs);

	void Cleanup();

private:

	GLuint m_viewProjectionBuffer = 0;
	GLuint m_transformBuffer = 0;
	GLuint m_cameraBuffer = 0;
};
