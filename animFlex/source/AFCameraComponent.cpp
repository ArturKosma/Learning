#include "AFCameraComponent.h"

#include "AFContent.h"

AFCameraComponent::AFCameraComponent()
{
	// Cache postprocess shaders.

	// Neutral.
	m_neutralShader = AFContent::Get().FindAsset<AFShader>("shader_neutral");

	// ID picker visualize.
	m_idPickerVizualiseShader = AFContent::Get().FindAsset<AFShader>("shader_idPickerVisualize");

	// UI fullscreen shader.
	m_uiFullscreenShader = AFContent::Get().FindAsset<AFShader>("shader_uiFullscreen");

	// Gaussian horizontal.
	m_postprocessShaders.push_back(AFContent::Get().FindAsset<AFPostprocessShader>("shader_gaussianHorizontal"));

	// Gaussian vertical.
	m_postprocessShaders.push_back(AFContent::Get().FindAsset<AFPostprocessShader>("shader_gaussianVertical"));

	// Fisheye.
	m_postprocessShaders.push_back(AFContent::Get().FindAsset<AFPostprocessShader>("shader_fisheye"));
}

AFCameraComponent::~AFCameraComponent()
{

}

void AFCameraComponent::SetFieldOfView(int newFOV)
{
	m_cameraProperties.fieldOfView = newFOV;
}

std::shared_ptr<AFShader> AFCameraComponent::GetNeutralShader() const
{
	return m_neutralShader;
}

std::shared_ptr<AFShader> AFCameraComponent::GetIDPickerVisualizeShader() const
{
	return m_idPickerVizualiseShader;
}

std::shared_ptr<AFShader> AFCameraComponent::GetUIFullScreenShader() const
{
	return m_uiFullscreenShader;
}

std::vector<std::shared_ptr<AFPostprocessShader>> AFCameraComponent::GetPostprocessShaders() const
{
	return m_postprocessShaders;
}

glm::mat4 AFCameraComponent::GetViewMatrix() const
{
	const glm::vec3 forward = glm::normalize(GetWorldRotationQuat() * glm::vec3(0.0f, 0.0f, -1.0f));
	const glm::vec3 target = GetWorldLocation() + forward;

	return glm::lookAt(GetWorldLocation(), target, glm::vec3(0.0f, 1.0f, 0.0f));
}

const FAFCameraProperties& AFCameraComponent::GetCameraProperties() const
{
	return m_cameraProperties;
}
