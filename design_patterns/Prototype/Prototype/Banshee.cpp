#include "Banshee.h"


Banshee::Banshee(unsigned InHealth, double InSpeed, float InAnger) :
	Monster(InHealth, InSpeed), Anger(InAnger)
{

}

Banshee::~Banshee()
{
}

Monster* Banshee::Clone()
{
	return new Banshee(Health, Speed, Anger);
}

std::string Banshee::GetDebugData()
{
	std::string str;

	str += "HP: ";
	str += std::to_string(Health);
	str += ", Speed: ";
	str += std::to_string(Speed);
	str += ", Anger: ";
	str += std::to_string(Anger);

	return str;
}
