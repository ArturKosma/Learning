#pragma once
#include <mutex>
class ii_onceFlag
{
public:

	void main();
	void fun();
	void call();

	ii_onceFlag();
	~ii_onceFlag();

private:

	std::once_flag once;
};

// #include "ii_onceFlag.h"
// 
// int main()
// {
// 	ii_onceFlag of;
// 	of.main();
// }