#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "AFWindow.h"

extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	inline void OnResizeCanvas(int newWidth, int newHeight)
	{
		AFWindow& window = AFWindow::GetInstance();
		window.OnWindowResize(newWidth, newHeight);
	}
}

#endif