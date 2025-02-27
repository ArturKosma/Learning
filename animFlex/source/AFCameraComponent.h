#pragma once
#include "AFPostprocessShader.h"
#include "AFSceneComponent.h"
#include "AFStructs.h"

class AFCameraComponent : public AFSceneComponent
{
public:

	AFCameraComponent();
	~AFCameraComponent() override;

	void SetFieldOfView(int newFOV);

	AFPostprocessShader GetNeutralShader() const;
	std::vector<AFPostprocessShader> GetPostprocessShaders() const;
	glm::mat4 GetViewMatrix() const;
	const AFCameraProperties& GetCameraProperties() const;

private:

	AFPostprocessShader m_neutralShader = AFPostprocessShader();
	std::vector<AFPostprocessShader> m_postprocessShaders = {};
	AFCameraProperties m_cameraProperties = AFCameraProperties();
};
