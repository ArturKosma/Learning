#include "pch.h"
#include "i_detach.h"

#include <thread>
#include <iostream>

i_detach::i_detach()
{
}


i_detach::~i_detach()
{
}

void i_detach::fun()
{
	std::cout << "lulz" << std::endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	fun();
}

void i_detach::operator()()
{
	fun();
}
