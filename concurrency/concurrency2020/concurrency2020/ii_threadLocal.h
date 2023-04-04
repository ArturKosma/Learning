#pragma once

#include <thread>

class ii_threadLocal
{
public:
	ii_threadLocal();
	~ii_threadLocal();

	void operator ()(unsigned InSomeNum);

	unsigned print();

private:

	static thread_local unsigned mynum;
};

// #include "ii_threadLocal.h"
// 
// int main()
// {
// 	ii_threadLocal obj0;
// 	ii_threadLocal obj1;
// 
// 	std::thread t0(std::thread(obj0, 111));
// 
// 	std::thread t1(std::thread(obj1, 222));
// 
// 	t0.join();
// 	t1.join();
// 
// 	obj0.print();
// 	obj1.print();
// }
