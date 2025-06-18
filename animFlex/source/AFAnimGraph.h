#pragma once
#include <string>

#include "AFObject.h"

class AFAnimGraph : public AFObject
{
public:

	void Compile(const std::string& graphString);
	void Evaluate(float deltaTime);
};
