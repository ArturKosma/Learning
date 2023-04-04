#pragma once

#include <mutex>

class ii_guardLock
{
public:

	ii_guardLock();

	~ii_guardLock();

	void main();
	void fun0();
	void fun1();

private:

	std::mutex Mutex;
};

// int main()
// {
// 	ii_guardLock gl;
// 	gl.main();
// }
