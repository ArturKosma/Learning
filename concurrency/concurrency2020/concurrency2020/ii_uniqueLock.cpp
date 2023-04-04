#include "pch.h"
#include "ii_uniqueLock.h"
#include <vector>


void ii_uniqueLock::main()
{
	std::vector<std::thread> threads;

	threads.emplace_back(std::thread([this]() {this->somefun(); }));
	threads.emplace_back(std::thread([this]() {this->somefun(); }));
	threads.emplace_back(std::thread([this]() {this->somefun(); }));
	threads.emplace_back(std::thread([this]() {this->somefun(); }));
	threads.emplace_back(std::thread([this]() {this->somefun(); }));

	for (size_t i = 0; i < threads.size(); ++i)
	{
		threads[i].join();
	}
}

void ii_uniqueLock::somefun()
{
	std::unique_lock<std::mutex>(Mut, std::defer_lock);

	printf("gettin rdy\n");

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	Mut.lock();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	printf("released!");

	Mut.unlock();
}

ii_uniqueLock::ii_uniqueLock()
{
}


ii_uniqueLock::~ii_uniqueLock()
{
}
