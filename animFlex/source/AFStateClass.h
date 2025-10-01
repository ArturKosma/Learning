#pragma once
#include <string>

class AFStateClass
{
public:

	virtual ~AFStateClass() = default;
	virtual bool CallFunctionByString(const std::string& funcName);
};
