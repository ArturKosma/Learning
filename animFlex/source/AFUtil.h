#pragma once

class AFUtil
{
	friend class AFWindow;

public:

	static float GetDeltaTime();

private:

	static float deltaTime;
};