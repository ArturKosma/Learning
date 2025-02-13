#include "AFCameraComponent.h"

AFCameraComponent::AFCameraComponent()
{
	// Init postprocess shaders.

	// Neutral.
	m_neutralShader.SetVertexShader("content/shaders/neutral.vert");
	m_neutralShader.SetFragmentShader("content/shaders/neutral.frag");
	m_neutralShader.LoadShaders();

	// Fisheye.
	AFPostprocessShader fisheye;
	fisheye.SetVertexShader("content/shaders/fisheye.vert");
	fisheye.SetFragmentShader("content/shaders/fisheye.frag");
	fisheye.LoadShaders();
	m_postprocessShaders.push_back(fisheye);

	// Gaussian horizontal.
	/*
	AFPostprocessShader gaussianHorizontal;
	gaussianHorizontal.SetVertexShader("content/shaders/gaussianHorizontal.vert");
	gaussianHorizontal.SetFragmentShader("content/shaders/gaussianHorizontal.frag");
	gaussianHorizontal.LoadShaders();
	m_postprocessShaders.push_back(gaussianHorizontal);

	// Gaussian vertical.
	AFPostprocessShader gaussianVertical;
	gaussianVertical.SetVertexShader("content/shaders/gaussianVertical.vert");
	gaussianVertical.SetFragmentShader("content/shaders/gaussianVertical.frag");
	gaussianVertical.LoadShaders();
	m_postprocessShaders.push_back(gaussianVertical);

	// Vignette.
	AFPostprocessShader vignette;
	vignette.SetVertexShader("content/shaders/vignette.vert");
	vignette.SetFragmentShader("content/shaders/vignette.frag");
	vignette.LoadShaders();
	m_postprocessShaders.push_back(vignette);*/
}

AFCameraComponent::~AFCameraComponent()
{

}

void AFCameraComponent::SetFieldOfView(int newFOV)
{
	m_cameraProperties.fieldOfView = newFOV;
}

AFPostprocessShader AFCameraComponent::GetNeutralShader() const
{
	return m_neutralShader;
}

std::vector<AFPostprocessShader> AFCameraComponent::GetPostprocessShaders() const
{
	return m_postprocessShaders;
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
