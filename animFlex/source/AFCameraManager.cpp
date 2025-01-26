#include "AFCameraManager.h"

void AFCameraManager::SetActiveCamera(AFCamera* newActiveCamera)
{
	if(!newActiveCamera)
	{
		return;
	}

	m_activeCamera = newActiveCamera;
}

AFCamera* AFCameraManager::GetActiveCamera() const
{
	return m_activeCamera;
}
