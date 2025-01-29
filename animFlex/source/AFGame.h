#pragma once
#include "AFScene.h"

class AFGame
{
	friend class AFApp;

public:

	bool Init();
	void Tick(float newDeltaTime);

	const AFScene& GetScene();

private:

	void BindInputs();

	void Input_FreeView_CameraYaw(float delta);
	void Input_FreeView_CameraPitch(float delta);

	void Input_FreeView_ForwardBackward(float axis);
	void Input_FreeView_RightLeft(float axis);
	void Input_FreeView_UpDown(float axis);

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();
};
