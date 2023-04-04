#pragma once
#include <thread>

class i_threadGuard
{
public:

	i_threadGuard(std::thread&& InThread);
	i_threadGuard(i_threadGuard&& InThreadGuard);
	i_threadGuard& operator=(i_threadGuard&& InThreadGuard);

	i_threadGuard() = delete;
	i_threadGuard(const i_threadGuard&) = delete;
	i_threadGuard& operator=(const i_threadGuard&) = delete;

	~i_threadGuard();

private:

	std::thread Thread;
};

// #include "pch.h"
// #include "i_threadGuard.h"
// 
// i_threadGuard fun()
// {
// 	i_threadGuard tg = i_threadGuard(std::thread([]() {std::this_thread::sleep_for(std::chrono::milliseconds(2000)); }));
// 	return tg;
// }
// 
// int main()
// {
// 	i_threadGuard tga = fun();
// 	i_threadGuard tg = std::move(tga);
// }