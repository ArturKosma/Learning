#pragma once
#include "Monster.h"
class Ghost :
	public Monster
{
public:
	Ghost(unsigned InHealth, double InSpeed, float InOpacity);
	~Ghost();

	virtual Monster* Clone() override;
	virtual std::string GetDebugData() override;

private:

	float Opacity = 0.5f;
};

