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
	AFPostprocessShader GetIDPickerVisualizeShader() const;
	AFPostprocessShader GetUIFullScreenShader() const;

	std::vector<AFPostprocessShader> GetPostprocessShaders() const;
	glm::mat4 GetViewMatrix() const;
	const AFCameraProperties& GetCameraProperties() const;

private:

	AFPostprocessShader m_neutralShader = AFPostprocessShader();
	AFPostprocessShader m_idPickerVizualiseShader = AFPostprocessShader();
	AFPostprocessShader m_uiFullscreenShader = AFPostprocessShader();
	std::vector<AFPostprocessShader> m_postprocessShaders = {};
	AFCameraProperties m_cameraProperties = AFCameraProperties();
};
