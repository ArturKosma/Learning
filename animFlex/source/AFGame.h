#pragma once

#include <map>

#include "AFCamera.h"

class AFGame
{
	friend class AFInput;

public:

	void Init();
	void Tick(float newDeltaTime);

	static float GetDeltaTime();

	static AFGame& GetInstance();
	const AFCamera& GetCamera() const;

	void ToggleTestState();
	bool GetTestState() const;

	AFGame(const AFGame&) = delete;
	AFGame& operator=(const AFGame&) = delete;
	AFGame(AFGame&&) = delete;
	AFGame& operator=(AFGame&&) = delete;

private:

	AFGame();
	~AFGame();

	void OnInput(int pressState);
	void OnCursorPosUpdate(double deltaX, double deltaY);
	void OnAxisInput(const std::map<unsigned int, float>& axisInputs);

	void SetTestState(bool newTestState);

	AFCamera m_camera = AFCamera();

	bool m_testState = false;
};
