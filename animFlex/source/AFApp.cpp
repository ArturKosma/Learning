#include "AFApp.h"
#include "AFInput.h"
#include "AFStructs.h"
#include "AFWindow.h"
#include "AFRenderer.h"
#include "AFGame.h"
#include "AFHelperInterface.h"
#include "AFTimerManager.h"
#include "AFConfig.h"
#include "AFUtility.h"
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

AFApp::AFApp()
{
	m_timerManager = new AFTimerManager();
	m_window = new AFWindow();
	m_renderer = new AFRenderer();
	m_game = new AFGame();
	m_helperInterface = new AFHelperInferface();

	constexpr int initWidth = 800;
	constexpr int initHeight = 600;

	if(!AFConfig::GetInstance().Init("config/config.ini"))
	{
		printf("%s\n", "Config Init() failed.");
		return;
	}

	SetWindowCallbacks();

	if (!m_window->Init(initWidth, initHeight))
	{
		printf("%s\n", "Window Init() failed.");
		return;
	}

	AFInput::Init(m_window->GetGLFWWindow());

	if (!m_renderer->Init(m_window->GetWidth(), m_window->GetHeight()))
	{
		printf("%s\n", "Renderer Init() failed.");
		return;
	}

	if (!m_game->Init())
	{
		printf("%s\n", "Game Init() failed.");
		return;
	}

	if(!m_helperInterface->Init(*m_window))
	{
		printf("%s\n", "Helper Interface Init() failed.");
		return;
	}

	AFTimerManager::GetInstance().Init();
}

AFApp::~AFApp()
{
	delete m_timerManager;
	delete m_window;
	delete m_renderer;
	delete m_game;
	delete m_helperInterface;
}

void AFApp::StartLoop()
{
	AFInput::BindAction("CloseApp", [this]() {glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true); }, EAFKeyAction::Pressed);
	AFInput::BindAction("Select", [this]()
	{
		const glm::ivec2& cursorPos = AFInput::GetInstance().GetCursorPos();
		const FAFPickID& colorID = m_renderer->ReadColorId(cursorPos.x, cursorPos.y);
		m_game->OnSelect(colorID);

	}, EAFKeyAction::Pressed);

#ifdef __EMSCRIPTEN__
	printf("Compiled with Emscripten.\n");

	// Set the web main loop to run at requestAnimationFrame() fps.
	emscripten_set_main_loop_arg([](void* InAppPtr)
		{
			AFApp* appPtr = static_cast<AFApp*>(InAppPtr);
			appPtr->Tick();
		}, this, 0, true);

#else
	while (!m_window->ShouldShutdown())
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

	m_window->PollEvents();
	AFInput::GetInstance().Tick();
	m_game->Tick(AFTimerManager::GetDeltaTime());
	m_renderer->Draw(m_game->GetScene().GetSceneData());

	AFAppData appData;
	CollectAppData(appData);

	m_helperInterface->Draw(appData, m_game->GetScene().GetSceneData());

	m_window->SwapBuffers();
}

void AFApp::CollectAppData(AFAppData& appData)
{
	appData.window = m_window->GetGLFWWindow();
	appData.width = m_window->GetWidth();
	appData.height = m_window->GetHeight();
}

void AFApp::SetWindowCallbacks()
{
	// Window resize.
	m_window->m_resizeCallback = [this](int newWidth, int newHeight)
		{
			OnWindowResize(newWidth, newHeight);
		};
}

void AFApp::OnWindowResize(int newWidth, int newHeight)
{
	// Update the frame buffer.

#ifdef __EMSCRIPTEN__
	glfwSetWindowSize(m_window->GetGLFWWindow(), newWidth, newHeight);
#endif

	m_renderer->SetSize(newWidth, newHeight);
}
