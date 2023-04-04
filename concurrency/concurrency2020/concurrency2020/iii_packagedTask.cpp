#include "pch.h"
#include "iii_packagedTask.h"
#include <thread>
#include <future>
#include <iostream>


void iii_packagedTask::main()
{
	std::packaged_task<std::string(std::string)> pt([this](std::string instr) {return this->somefun(instr); });

	std::future<std::string> fut = pt.get_future();

	std::thread th(std::move(pt), "SOME STRING SO FAR");
	th.detach();

	std::cout << fut.get() << std::endl;
}

std::string iii_packagedTask::somefun(std::string inStr)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	return inStr;
}

iii_packagedTask::iii_packagedTask()
{
}


iii_packagedTask::~iii_packagedTask()
{
}
