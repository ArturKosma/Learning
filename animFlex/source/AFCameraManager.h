#pragma once

#include "AFCamera.h"

class AFCameraManager
{
public:

	void SetActiveCamera(class AFCamera* newActiveCamera);
	AFCamera* GetActiveCamera() const;

private:

	AFCamera* m_activeCamera = nullptr;
};
