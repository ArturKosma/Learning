#pragma once
#include <mutex>
class ii_uniqueLock
{
public:

	void main();

	void somefun();


	ii_uniqueLock();
	~ii_uniqueLock();

private:

	std::mutex Mut;
};

// int main()
// {
// 	ii_uniqueLock ulock;
// 
// 	ulock.main();
// }