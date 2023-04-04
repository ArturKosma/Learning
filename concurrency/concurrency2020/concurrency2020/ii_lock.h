#pragma once
#include <mutex>
class ii_lock
{
public:

	void main();

	void single();
	void dual();

	ii_lock();
	~ii_lock();

private:

	std::mutex first;
	std::mutex second;
};

// #include "ii_lock.h"
// 
// int main()
// {
// 	ii_lock	lok;
// 	lok.main();
// }