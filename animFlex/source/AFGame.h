#pragma once
#include "AFScene.h"

class AFGame
{
	friend class AFApp;

public:

	bool Init();
	void Tick(float newDeltaTime);

	static AFGame* GetGame();
	const AFScene& GetScene();

private:

	void OnSelect(const FAFPickID& pickID);
	void OnHover(const FAFPickID& pickID);

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();

	AFObject* m_currentHover = nullptr;
	uint8_t m_currentHoverElement = -1;
};
