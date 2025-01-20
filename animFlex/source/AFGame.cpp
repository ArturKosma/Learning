#include "AFGame.h"

AFGame& AFGame::GetInstance()
{
	static AFGame gameInstance;
	return gameInstance;
}

void AFGame::Init()
{
}

bool AFGame::GetTestState() const
{
	return m_testState;
}

AFGame::AFGame()
{
}

AFGame::~AFGame()
{
}

void AFGame::OnInput(int pressState)
{
	if(pressState == 1)
	{
		SetTestState(!m_testState);
	}
}

void AFGame::SetTestState(bool newTestState)
{
	m_testState = newTestState;
}
