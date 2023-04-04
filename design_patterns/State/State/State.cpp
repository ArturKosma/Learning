#include "MainWindow.h"
#include <iostream>

void MyForward_Pressed()
{
	std::wcout << "Pressed forward!" << std::endl;
}
void MyRight_Pressed()
{
	std::wcout << "Pressed right!" << std::endl;
}
void MyBack_Pressed()
{
	std::wcout << "Pressed back!" << std::endl;
}
void MyLeft_Pressed()
{
	std::wcout << "Pressed left!" << std::endl;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// Initialize window.
	MainWindow::Initialize(hInstance, pCmdLine, nCmdShow);

	InputManager::GetInstance()->BindInput(TEXT("Forward"), EInputActionType::INPUTACTIONTYPE_Pressed, &MyForward_Pressed);
	InputManager::GetInstance()->BindInput(TEXT("Right"), EInputActionType::INPUTACTIONTYPE_Pressed, &MyRight_Pressed);
	InputManager::GetInstance()->BindInput(TEXT("Back"), EInputActionType::INPUTACTIONTYPE_Pressed, &MyBack_Pressed);
	InputManager::GetInstance()->BindInput(TEXT("Left"), EInputActionType::INPUTACTIONTYPE_Pressed, &MyLeft_Pressed);

	// Game loop.
	MainWindow::StartMessageLoop();

	return 0;
}