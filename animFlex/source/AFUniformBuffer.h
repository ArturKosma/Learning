#pragma once 

#include <glm/glm.hpp>
#include <glad/glad.h>

class AFUniformBuffer
{
public:

	void Init();

	void UploadUBOData(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	void Cleanup();

private:

	GLuint m_uboBuffer = 0;
};