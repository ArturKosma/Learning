#pragma once
#include "AFComponent.h"
#include "AFStructs.h"

class AFCameraComponent : public AFComponent
{
public:

	void SetFieldOfView(int newFOV);

	glm::mat4 GetViewMatrix() const;
	const AFCameraProperties& GetCameraProperties() const;

private:

	AFCameraProperties m_cameraProperties = AFCameraProperties();
};
