#include "AFGame.h"
#include "AFInput.h"
#include "AFUtil.h"

#include <cmath>
#include <cstdio>
#include <string>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

AFGame& AFGame::GetInstance()
{
	static AFGame gameInstance;
	return gameInstance;
}

void AFGame::Init()
{
}

void AFGame::Tick(float newDeltaTime)
{
	m_camera.Tick(newDeltaTime);
}

float AFGame::GetDeltaTime()
{
	return AFUtil::GetDeltaTime();
}

const AFCamera& AFGame::GetCamera() const
{
	return m_camera;
}

void AFGame::ToggleTestState()
{
	SetTestState(!m_testState);
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
	/*if(pressState == 1)
	{
		SetTestState(!m_testState);
	}*/
}

void AFGame::OnCursorPosUpdate(double deltaX, double deltaY)
{
	if(AFInput::GetMouseLock())
	{
		constexpr float cameraRotStrength = 5.0f;

		m_camera.AddRotation(glm::vec3(static_cast<float>(deltaY) * GetDeltaTime() * cameraRotStrength,
			static_cast<float>(deltaX) * GetDeltaTime() * cameraRotStrength, 0.0f));
	}
}

void AFGame::OnAxisInput(const std::map<unsigned int, float>& axisInputs)
{
	if (AFInput::GetMouseLock())
	{
		const float forwardBackward = axisInputs.at(0);
		const float leftRight = axisInputs.at(1);
		const float upDown = axisInputs.at(2);

		const glm::quat& cameraRotQuat = m_camera.GetRotation();

		const glm::vec3& forward = cameraRotQuat * glm::vec3(0.0f, 0.0f, -1.0f);
		const glm::vec3& right = cameraRotQuat * glm::vec3(1.0f, 0.0f, 0.0f);
		const glm::vec3& up = cameraRotQuat * glm::vec3(0.0f, 1.0f, 0.0f);

		const glm::vec3& offsetToAdd = (forward * forwardBackward) +
			(right * leftRight) +
			(up * upDown);

		m_camera.AddMovementInput(offsetToAdd);
	}
}

void AFGame::SetTestState(bool newTestState)
{
	m_testState = newTestState;
}
