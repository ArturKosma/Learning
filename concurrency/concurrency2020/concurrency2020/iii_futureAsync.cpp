#include "pch.h"
#include "iii_futureAsync.h"
#include <iostream>


void iii_futureAsync::main()
{
	fut = std::async(&iii_futureAsync::fun, this);

	std::cout << fut.get() << std::endl;
}

int iii_futureAsync::fun()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	return 654;
}

iii_futureAsync::iii_futureAsync()
{
}


iii_futureAsync::~iii_futureAsync()
{
}
