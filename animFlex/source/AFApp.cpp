#include "AFApp.h"
#include "AFInput.h"
#include "AFStructs.h"
#include "AFWindow.h"
#include "AFRenderer.h"
#include "AFGame.h"
#include "AFHelperInterface.h"
#include "AFTimerManager.h"
#include "AFConfig.h"
#include "AFContent.h"
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
	m_helperInterface = new AFHelperInterface();
	m_appData = new FAFAppData();

	constexpr int initWidth = 800;
	constexpr int initHeight = 600;

	if (!AFConfig::GetInstance().Init("config/config.ini"))
	{
		printf("%s\n", "Config Init() failed.");
		return;
	}

	if (!m_window->Init(initWidth, initHeight))
	{
		printf("%s\n", "Window Init() failed.");
		return;
	}

	// Init content - load all necessary assets.
	if (!AFContent::Get().Init())
	{
		printf("%s\n", "Content Init() failed.");
		return;
	}

	SetWindowCallbacks();
	m_window->ResizeToMonitor();

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

	/*if (!m_helperInterface->Init(*m_window))
	{
		printf("%s\n", "Helper Interface Init() failed.");
		return;
	}*/

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
	GetGame()->BeginPlay();
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

void AFApp::CloseApp()
{
	glfwSetWindowShouldClose(m_window->GetGLFWWindow(), true);
}

AFApp& AFApp::GetInstance()
{
	static AFApp appInstance;
	return appInstance;
}

glm::ivec2 AFApp::GetWindowSize()
{
	return { GetInstance().m_window->GetWidth(), GetInstance().m_window->GetHeight() };
}

void AFApp::SetCursorHidden(bool hidden)
{
	if (hidden)
	{
#ifdef __EMSCRIPTEN__
		emscripten_request_pointerlock("#canvas", true);
#else
		glfwSetInputMode(GetWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
		{
			int w_px, h_px;
			glfwGetWindowSize(GetWindow()->GetGLFWWindow(), &w_px, &h_px);
			double centerX = w_px * 0.5;
			double centerY = h_px * 0.5;

			// Initially warp cursor to center to avoid init bump.
			glfwSetCursorPos(GetWindow()->GetGLFWWindow(), centerX, centerY);

			glfwSetInputMode(GetWindow()->GetGLFWWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		}
#endif
	}
	else
	{
#ifdef __EMSCRIPTEN__
		emscripten_exit_pointerlock();
#else
		glfwSetInputMode(GetWindow()->GetGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (glfwRawMouseMotionSupported())
		{
			// Turn off raw.
			glfwSetInputMode(GetWindow()->GetGLFWWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
		}
#endif
	}
}

void AFApp::Tick()
{
	AFTimerManager::GetInstance().DeltaCalc();
	const float delta = AFTimerManager::GetDeltaTime();
	AFTimerManager::GetInstance().Tick(delta);

	m_window->PollEvents();
	AFInput::GetInstance().Tick();
	m_game->Tick(delta);

	const FAFSceneData& sceneData = m_game->GetScene().GetSceneData();
	CollectAppData(*m_appData);

	m_renderer->Draw(sceneData, *m_appData);
	//m_helperInterface->Draw(sceneData, *m_appData);

	m_window->SwapBuffers();

	// Mouse hover.
	const glm::ivec2& cursorPos = AFInput::GetInstance().GetCursorPos();
	const FAFPickID& colorID = m_renderer->ReadColorId(cursorPos.x, cursorPos.y);
	m_game->OnHover(colorID);
}

void AFApp::CollectAppData(FAFAppData& appData)
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

#ifdef __EMSCRIPTEN__
	glfwSetWindowSize(m_window->GetGLFWWindow(), newWidth, newHeight);
#endif

	m_renderer->SetSize(newWidth, newHeight);
}

AFGame* AFApp::GetGame() const
{
	return m_game;
}

AFRenderer* AFApp::GetRenderer() const
{
	return m_renderer;
}

AFWindow* AFApp::GetWindow() const
{
	return m_window;
}
