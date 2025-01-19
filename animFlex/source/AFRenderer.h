#pragma once

#include <glad/glad.h>

class AFRenderer
{
	friend class AFWindow;

public:

	void TickRender(float deltaTime);
	void OnWindowResize(int newWidth, int newHeight);

private:

	AFRenderer();
	~AFRenderer();
};