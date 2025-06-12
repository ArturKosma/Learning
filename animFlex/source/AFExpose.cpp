#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "AFApp.h"

extern "C"
{
	EMSCRIPTEN_KEEPALIVE
	void OnResizeCanvas(int newWidth, int newHeight)
	{
		AFApp& app = AFApp::GetInstance();
		//printf("%d\n", newHeight);
		app.OnWindowResize(newWidth, newHeight);
	}

	EMSCRIPTEN_KEEPALIVE
	void OnGraphUpdate(const char* graphState)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnGraphUpdate(graphState);
	}
}

#endif