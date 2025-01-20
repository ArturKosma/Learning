#pragma once

class AFGame
{
	friend class AFInput;

public:

	static AFGame& GetInstance();

	void Init();

	bool GetTestState() const;

	AFGame(const AFGame&) = delete;
	AFGame& operator=(const AFGame&) = delete;
	AFGame(AFGame&&) = delete;
	AFGame& operator=(AFGame&&) = delete;

private:

	AFGame();
	~AFGame();

	void OnInput(int pressState);

	void SetTestState(bool newTestState);

	bool m_testState = false;
};
