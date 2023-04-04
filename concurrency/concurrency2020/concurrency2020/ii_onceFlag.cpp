#include "pch.h"
#include "ii_onceFlag.h"


void ii_onceFlag::main()
{
	std::thread t1 = std::thread([this]() {this->call(); });
	std::thread t2 = std::thread([this]() {this->call(); });

	t1.join();
	t2.join();
}

void ii_onceFlag::fun()
{
	std::printf("am i single?\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));
}

void ii_onceFlag::call()
{
	std::call_once(once, &ii_onceFlag::fun, this);
}

ii_onceFlag::ii_onceFlag()
{
}


ii_onceFlag::~ii_onceFlag()
{
}
