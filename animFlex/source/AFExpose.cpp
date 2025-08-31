#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include "AFApp.h"
#include "AFGame.h"
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
		app.OnWindowResize(newWidth, newHeight);
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
	void OnStateConnectionCreated(const char* msg)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnStateConnectionCreated(msg);
	}

	EMSCRIPTEN_KEEPALIVE
	void OnStateConnectionRemoved(const char* msg)
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->OnStateConnectionRemoved(msg);
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

	EMSCRIPTEN_KEEPALIVE
	const char* GetLastActiveStates()
	{
		const std::string lastActiveStates = AFEvaluator::Get().GetLastActiveStates();

		char* buffer = (char*)malloc(lastActiveStates.length() + 1);
		strcpy(buffer, lastActiveStates.c_str());

		return buffer;
	}

	EMSCRIPTEN_KEEPALIVE
	void FreeLastActiveStates(char* ptr)
	{
		free(ptr);
	}

	EMSCRIPTEN_KEEPALIVE
	void BeginPlayFromJS()
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->BeginPlay();
	}

	EMSCRIPTEN_KEEPALIVE
	void Possess()
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->SetControlMode(EAFControlMode::ActionRPG);
	}

	EMSCRIPTEN_KEEPALIVE
	void UnPossess()
	{
		AFApp& app = AFApp::GetInstance();
		app.GetGame()->SetControlMode(EAFControlMode::Editor);
	}

	EMSCRIPTEN_KEEPALIVE
	const char* GetPlayrateStatus()
	{
		const std::string playrateStatus = AFEvaluator::Get().GetPlayrateStatus();

		char* buffer = (char*)malloc(playrateStatus.length() + 1);
		strcpy(buffer, playrateStatus.c_str());

		return buffer;
	}

	EMSCRIPTEN_KEEPALIVE
	void FreePlayrateStatus(char* ptr)
	{
		free(ptr);
	}

	EMSCRIPTEN_KEEPALIVE
	void PlayrateSlower()
	{
		AFEvaluator::Get().PlayrateSlower();
	}

	EMSCRIPTEN_KEEPALIVE
	void PlayrateToggle()
	{
		AFEvaluator::Get().PlayrateToggle();
	}

	EMSCRIPTEN_KEEPALIVE
	void PlayrateFaster()
	{
		AFEvaluator::Get().PlayrateFaster();
	}
}
#endif
