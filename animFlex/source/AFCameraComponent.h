#pragma once
#include "AFComponent.h"
#include "AFPostprocessShader.h"
#include "AFStructs.h"

class AFCameraComponent : public AFComponent
{
public:

	AFCameraComponent();
	~AFCameraComponent() override;

	void SetFieldOfView(int newFOV);

	AFPostprocessShader GetPostprocessShader() const;
	glm::mat4 GetViewMatrix() const;
	const AFCameraProperties& GetCameraProperties() const;

private:

	AFPostprocessShader m_postprocessShader = AFPostprocessShader();
	AFCameraProperties m_cameraProperties = AFCameraProperties();
};
