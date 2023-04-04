#pragma once
#include <mutex>


class iii_conditionalVar
{
public:

	void main();
	void separateThread();

	std::condition_variable var;
	std::mutex mtx;

	iii_conditionalVar();
	~iii_conditionalVar();
};

// #include "iii_conditionalVar.h"
// 
// int main()
// {
// 	iii_conditionalVar var;
// 
// 	var.main();
// }