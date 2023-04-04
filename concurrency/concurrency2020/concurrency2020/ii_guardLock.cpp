#include "pch.h"
#include "ii_guardLock.h"
#include <vector>

ii_guardLock::ii_guardLock()
{

}

ii_guardLock::~ii_guardLock()
{
}

void ii_guardLock::main()
{
	std::vector<std::thread> threads;

	threads.emplace_back(std::thread([this]() {this->fun0(); }));
	threads.emplace_back(std::thread([this]() {this->fun1(); }));

	threads[0].join();
	threads[1].join();
}

void ii_guardLock::fun0()
{
	std::lock_guard<std::mutex> guard(Mutex);

	for (size_t i = 0; i < 3; ++i)
	{
		printf("doing fun0\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

void ii_guardLock::fun1()
{
	std::lock_guard<std::mutex> guard(Mutex);

	for (size_t i = 0; i < 3; ++i)
	{
		printf("doing fun1\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
