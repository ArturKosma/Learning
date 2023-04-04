#include "pch.h"
#include "MyStack.h"
#include <iostream>

int main()
{
	MyStack<int> mystack;
	int cont;

	mystack.Push(444);
	mystack.Push(555);
	mystack.Push(666);

	std::thread t1(&MyStack<int>::Pop, &mystack, std::ref(cont));
	std::thread t2(&MyStack<int>::Pop, &mystack, std::ref(cont));
	//std::thread t3(&MyStack<int>::Pop, &mystack, std::ref(cont));

	t1.join();
	t2.join();
	//t3.join();

	std::cout << mystack.Read() << std::endl;
}
