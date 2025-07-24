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

	// #hack
	// Signals from the frontend probably shouldn't be here.
	// @todo fixme.
	void OnNodeCreated(const char* msg);
	void OnNodeUpdated(const char* msg);
	void OnNodeRemoved(const char* msg);

	void OnStateConnectionCreated(const char* msg);
	void OnStateConnectionRemoved(const char* msg);

private:

	void OnSelect(const FAFPickID& pickID);
	void OnHover(const FAFPickID& pickID);

	AFGame();
	~AFGame();

	AFScene m_scene = AFScene();

	std::shared_ptr<AFObject> m_currentHover = nullptr;
	uint8_t m_currentHoverElement = -1;

	float placeholderAccum = 0.0f;
};