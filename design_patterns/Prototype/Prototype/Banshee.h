#pragma once
#include "Monster.h"
class Banshee :
	public Monster
{
public:
	Banshee(unsigned InHealth, double InSpeed, float InAnger);
	~Banshee();


	virtual Monster* Clone() override;
	virtual std::string GetDebugData() override;

private:

	float Anger = 1000.0f;
};

