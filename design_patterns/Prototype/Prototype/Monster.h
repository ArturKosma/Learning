#pragma once

#include <string>

class Monster
{
public:

	virtual ~Monster();

	virtual Monster* Clone() = 0;
	virtual std::string GetDebugData() = 0;

protected:

	Monster(unsigned InHealth, double InSpeed);

	unsigned Health = 100;
	double Speed = 20.0;
};

