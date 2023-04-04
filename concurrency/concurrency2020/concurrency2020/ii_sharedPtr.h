#pragma once

#include <iostream>


class ii_sharedPtr
{
public:
	ii_sharedPtr();
	~ii_sharedPtr();
};

// #include <memory>
// #include "ii_sharedPtr.h"
// #include <thread>
// 
// std::shared_ptr<ii_sharedPtr> fun(void)
// {
// 	std::shared_ptr<ii_sharedPtr> ptr0(new ii_sharedPtr);
// 
// 	return ptr0;
// }
// 
// int main()
// {
// 	std::shared_ptr<ii_sharedPtr> ptr1;
// 	/*ptr1 = */fun();
// 	std::shared_ptr<ii_sharedPtr> ptr2;
// 
// 	printf("yes\n");
// 
// 	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
// }