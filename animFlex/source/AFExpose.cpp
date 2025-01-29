#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "AFApp.h"

extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	inline void OnResizeCanvas(int newWidth, int newHeight)
	{
		AFApp& app = AFApp::GetInstance();
		app.OnWindowResize(newWidth, newHeight);
	}
}

#endif