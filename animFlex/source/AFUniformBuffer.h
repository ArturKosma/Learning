#pragma once 

#include <glm/glm.hpp>
#include <glad/glad.h>

class AFUniformBuffer
{
public:

	void Init();

	void UploadViewProjection(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	void UploadViewRotation(glm::mat4 viewRotationMatrix);
	void UploadOrthoProjection(glm::mat4 orthoProjectionMatrix);
	void UploadOrthoPixelProjection(glm::mat4 orthoProjectionMatrix);
	void UploadInverseViewProjection(glm::mat4 inverseViewMatrix, glm::mat4 inverseProjectionMatrix);
	void UploadTransform(glm::mat4 modelMatrix);
	void UploadCamera(glm::mat4 cameraTrs);
	void UploadRenderProperties(glm::mat4 renderProperties);
	void UploadUITransform(glm::mat4 uiTransform);
	void UploadJointsMatrices(const std::vector<glm::mat4>& jointsMatrices);
	void UploadJointsDualQuatMatrices(const std::vector<glm::mat4>& jointsDualQuatMatrices);

	void Cleanup();

private:

	GLuint m_viewProjectionBuffer = 0;
	GLuint m_inverseViewProjectionBuffer = 0;
	GLuint m_transformBuffer = 0;
	GLuint m_cameraBuffer = 0;
	GLuint m_renderPropertiesBuffer = 0;
	GLuint m_uiTransformBuffer = 0;
	GLuint m_viewRotationBuffer = 0;
	GLuint m_orthoProjectionBuffer = 0;
	GLuint m_orthoPixelProjectionBuffer = 0;
	GLuint m_jointMatricesBuffer = 0;
	GLuint m_jointDualQuatMatricesBuffer = 0;
};
