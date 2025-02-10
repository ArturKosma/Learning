#include "AFCameraComponent.h"

AFCameraComponent::AFCameraComponent()
{
	// Init postprocess shader.
	m_postprocessShader.SetVertexShader("content/shaders/postprocess.vert");
	m_postprocessShader.SetFragmentShader("content/shaders/postprocess.frag");
	m_postprocessShader.LoadShaders();

	// Set far/near plane uniforms.
	m_postprocessShader.SetUniform1f("u_zNear", GetCameraProperties().near);
	m_postprocessShader.SetUniform1f("u_zFar", GetCameraProperties().far);
}

AFCameraComponent::~AFCameraComponent()
{

}

void AFCameraComponent::SetFieldOfView(int newFOV)
{
	m_cameraProperties.fieldOfView = newFOV;
}

AFPostprocessShader AFCameraComponent::GetPostprocessShader() const
{
	return m_postprocessShader;
}

glm::mat4 AFCameraComponent::GetViewMatrix() const
{
	const glm::vec3 forward = glm::normalize(GetWorldRotationQuat() * glm::vec3(0.0f, 0.0f, -1.0f));
	const glm::vec3 target = GetWorldLocation() + forward;

	return glm::lookAt(GetWorldLocation(), target, glm::vec3(0.0f, 1.0f, 0.0f));
}

const AFCameraProperties& AFCameraComponent::GetCameraProperties() const
{
	return m_cameraProperties;
}
