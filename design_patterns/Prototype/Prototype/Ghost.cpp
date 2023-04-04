#include "Ghost.h"



Ghost::Ghost(unsigned InHealth, double InSpeed, float InOpacity) :
	Monster(InHealth, InSpeed), Opacity(InOpacity)
{
}


Ghost::~Ghost()
{
}

Monster* Ghost::Clone()
{
	return new Ghost(Health, Speed, Opacity);
}

std::string Ghost::GetDebugData()
{
	std::string str;

	str += "HP: ";
	str += std::to_string(Health);
	str += ", Speed: ";
	str += std::to_string(Speed);
	str += ", Opacity: ";
	str += std::to_string(Opacity);

	return str;
}
