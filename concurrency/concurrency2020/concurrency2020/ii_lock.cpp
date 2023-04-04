#include "pch.h"
#include "ii_lock.h"
#include <vector>


void ii_lock::main()
{
	std::vector<std::thread> threads;

	threads.emplace_back(std::thread([this]() {this->single(); }));
	threads.emplace_back(std::thread([this]() {this->dual(); }));

	threads[0].join();
	threads[1].join();
}

void ii_lock::single()
{
	first.lock();

	printf("doing me single!\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	first.unlock();
}

void ii_lock::dual()
{
	std::lock<std::mutex, std::mutex>(first, second);

	std::lock_guard<std::mutex>(first, std::adopt_lock);
	std::lock_guard<std::mutex>(second, std::adopt_lock);

	printf("doing me dual!\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
}

ii_lock::ii_lock()
{
}


ii_lock::~ii_lock()
{
}
