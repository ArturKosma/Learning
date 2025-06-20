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

	EMSCRIPTEN_KEEPALIVE
	void OnNodeCreated(const char* msg)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnNodeCreated(msg);
	}

	EMSCRIPTEN_KEEPALIVE
	void OnNodeUpdated(const char* msg)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnNodeUpdated(msg);
	}

	EMSCRIPTEN_KEEPALIVE
	void OnNodeRemoved(const char* msg)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnNodeRemoved(msg);
	}
}

#endif