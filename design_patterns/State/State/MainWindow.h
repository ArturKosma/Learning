#pragma once

#include <fcntl.h>
#include <io.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "InputManager.h"

class MainWindow
{
public:

	static HWND Initialize(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);
	static void StartMessageLoop();

private:

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	MainWindow();
};
