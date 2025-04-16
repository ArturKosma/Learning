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

	void OnSelect(const FAFPickID& pickID);

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();
};
