#pragma once
#include <future>

class iii_futureAsync
{
public:

	std::future<int> fut;

	void main();

	int fun();

	iii_futureAsync();
	~iii_futureAsync();
};


// #include "iii_futureAsync.h"
// 
// int main()
// {
// 	iii_futureAsync myvar;
// 
// 	myvar.main();
// }
