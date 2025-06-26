#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "AFApp.h"
#include <cstring>
#include <cstdlib>

#include "AFAnimState.h"
#include "AFPlayerPawn.h"
#include "AFEvaluator.h"

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

	EMSCRIPTEN_KEEPALIVE
	const char* GetLastActiveSockets()
	{
		const std::string lastActiveSockets = AFEvaluator::Get().GetLastActiveSockets();

		char* buffer = (char*)malloc(lastActiveSockets.length() + 1);
		strcpy(buffer, lastActiveSockets.c_str());

		return buffer;
	}

	EMSCRIPTEN_KEEPALIVE
	void FreeLastActiveSockets(char* ptr)
	{
		free(ptr);
	}
}
#endif
