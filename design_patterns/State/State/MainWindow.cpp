#include "MainWindow.h"

HWND MainWindow::Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Open a console.
	AllocConsole();

	// Send messages from COUT and CIN to stream.
	FILE* stream;
	freopen_s(&stream, "CONIN$", "r+t", stdin);
	freopen_s(&stream, "CONOUT$", "w+t", stdout);
	freopen_s(&stream, "CONOUT$", "w+t", stderr);

	// Enable UTF-16
	_setmode(_fileno(stdout), _O_U16TEXT);

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	// Create the window.
	HWND hwnd = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Learn to Program Windows",    // Window text
		WS_OVERLAPPEDWINDOW,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
}

void MainWindow::StartMessageLoop()
{
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK MainWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	InputManager* inputManager = InputManager::GetInstance();

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);

		break;
	}
	case WM_KEYDOWN:
	{
		inputManager->UpdateInput((unsigned)wParam, true);
		break;
	}
	case WM_KEYUP:
	{
		inputManager->UpdateInput((unsigned)wParam, false);
		break;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
