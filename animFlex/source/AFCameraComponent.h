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

	std::shared_ptr<AFShader> GetNeutralShader() const;
	std::shared_ptr<AFShader> GetIDPickerVisualizeShader() const;
	std::shared_ptr<AFShader> GetUIFullScreenShader() const;

	std::vector<std::shared_ptr<AFPostprocessShader>> GetPostprocessShaders() const;
	glm::mat4 GetViewMatrix() const;
	const FAFCameraProperties& GetCameraProperties() const;

private:

	std::shared_ptr<AFShader> m_neutralShader = nullptr;
	std::shared_ptr<AFShader> m_idPickerVizualiseShader = nullptr;
	std::shared_ptr<AFShader> m_uiFullscreenShader = nullptr;
	std::vector<std::shared_ptr<AFPostprocessShader>> m_postprocessShaders = {};
	FAFCameraProperties m_cameraProperties = FAFCameraProperties();
};
