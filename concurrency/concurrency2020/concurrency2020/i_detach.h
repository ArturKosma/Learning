#pragma once
class i_detach
{
public:
	i_detach();
	~i_detach();

	void fun();

	void operator()();
};

// #include "pch.h"
// #include "i_detach.h"
// 
// #include <thread>
// 
// int main()
// {
// 	i_detach myobj;
// 	std::thread mythread = std::thread(myobj);
// 	mythread.detach();
// 
// 	std::this_thread::sleep_for(std::chrono::milliseconds(4500));
// }

