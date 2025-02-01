#pragma once 

#include <glm/glm.hpp>
#include <glad/glad.h>

class AFUniformBuffer
{
public:

	void Init();

	void UploadMatrices(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::mat4 modelMatrix);
	void UploadCamera(glm::mat4 cameraTrs);

	void Cleanup();

private:

	GLuint m_matricesBuffer = 0;
	GLuint m_cameraBuffer = 0;
};
