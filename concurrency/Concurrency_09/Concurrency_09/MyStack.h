#pragma once
#include <stack>
#include <string>
#include <mutex>

template<typename TStackType>
class MyStack
{
public:

	MyStack();
	~MyStack();

	void Push(const TStackType& InElement);
	bool Pop(TStackType& InContainer);
	std::string Read();
	bool Empty() const;

private:

	mutable std::mutex Mut;
	std::stack<TStackType> Stack;
};

template<typename TStackType>
MyStack<TStackType>::~MyStack()
{

}

template<typename TStackType>
MyStack<TStackType>::MyStack()
{

}

template<typename TStackType>
void MyStack<TStackType>::Push(const TStackType& InElement)
{
	std::lock_guard<std::mutex> guard(Mut);
	Stack.push(InElement);
}

template<typename TStackType>
bool MyStack<TStackType>::Pop(TStackType& InContainer)
{
	std::lock_guard<std::mutex> guard(Mut);

	if (Stack.empty())
	{
		return false;
	}

	InContainer = Stack.top();
	Stack.pop();
	return true;
}

template<typename TStackType>
std::string MyStack<TStackType>::Read()
{
	std::string retStr;
	MyStack<TStackType> dummyStack;
	TStackType dummy;

	while (!Empty())
	{
		Pop(dummy);
		dummyStack.Push(dummy);
		retStr += std::to_string(dummy);
		retStr += " ";
	}

	// Fill back.
	while (!dummyStack.Empty())
	{
		dummyStack.Pop(dummy);
		Push(dummy);
	}

	return retStr;
}

template<typename TStackType>
bool MyStack<TStackType>::Empty() const
{
	std::lock_guard<std::mutex> guard(Mut);
	return Stack.empty();
}
