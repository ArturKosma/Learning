#include "AFApp.h"

#include "AFInput.h"
#include "AFStructs.h"

AFApp::AFApp()
{
	constexpr int initWidth = 800;
	constexpr int initHeight = 600;

	if(!m_window.Init(initWidth, initHeight))
	{
		printf("%s\n", "Window Init() failed.");
		return;
	}

	SetWindowCallbacks();

	if (!m_game.Init())
	{
		printf("%s\n", "Game Init() failed.");
		return;
	}

	if(!m_renderer.Init(initWidth, initHeight))
	{
		printf("%s\n", "Renderer Init() failed.");
		return;
	}

	AFTimerManager::GetInstance().Init();
}

AFApp::~AFApp()
{
}

void AFApp::StartLoop()
{
#ifdef __EMSCRIPTEN__
	printf("Running on Emscripten.\n");

	// Set the web main loop to run at requestAnimationFrame() fps.
	emscripten_set_main_loop_arg([](void* InWindowPtr)
		{
			AFWindow* windowPtr = static_cast<AFWindow*>(InWindowPtr);
			windowPtr->Tick();
		}, this, 0, true);

#else
	printf("Not running on Emscripten.\n");

	while (!m_window.ShouldShutdown())
	{
		Tick();
	}
#endif
}

AFApp& AFApp::GetInstance()
{
	static AFApp appInstance;
	return appInstance;
}

void AFApp::Tick()
{
	// Calculate delta time.
	AFTimerManager::GetInstance().DeltaCalc();

	AFInput::Tick();
	m_game.Tick(AFTimerManager::GetDeltaTime());
	m_renderer.Draw(m_game.GetScene().GetSceneData());

	AFAppData appData;
	CollectAppData(appData);
	m_helperInterface.Draw(appData);

	m_window.SwapBuffers();
	m_window.PollEvents();
}

void AFApp::CollectAppData(AFAppData& appData)
{
	appData.window = m_window.GetGLFWWindow();
	appData.activeCamera = m_game.GetCameraManager().GetActiveCamera();
	appData.triangles = m_scene.GetTotalNumTriangles();
	appData.width = m_window.GetWidth();
	appData.height = m_window.GetHeight();
}

void AFApp::SetWindowCallbacks()
{
	// Window resize.
	m_window.m_resizeCallback = [this](int newWidth, int newHeight)
		{
			OnWindowResize(newWidth, newHeight);
		};
}

void AFApp::OnWindowResize(int newWidth, int newHeight)
{
	// Update the frame buffer.
	m_renderer.SetSize(newWidth, newHeight);
}
