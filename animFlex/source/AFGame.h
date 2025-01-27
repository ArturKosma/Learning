#pragma once
#include "AFScene.h"

#include <map>

class AFGame
{
	friend class AFApp;

public:

	bool Init();
	void Tick(float newDeltaTime);

	class AFCameraManager* GetCameraManager() const;

	const AFScene& GetScene();

private:

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();

	void OnCursorPosUpdate(double deltaX, double deltaY);
	void OnAxisInput(const std::map<unsigned int, float>& axisInputs);
	void OnScrollUpdate(double deltaX, double deltaY);

	class AFCameraManager* m_cameraManager = nullptr;
};
