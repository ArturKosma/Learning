#include "pch.h"
#include "i_threadGuard.h"

i_threadGuard::i_threadGuard(std::thread&& InThread) : Thread(std::move(InThread))
{

}

i_threadGuard::i_threadGuard(i_threadGuard&& InThreadGuard) : Thread(std::move(InThreadGuard.Thread))
{

}

i_threadGuard& i_threadGuard::operator=(i_threadGuard&& InThreadGuard)
{
	Thread = std::move(InThreadGuard.Thread);
	return *this;
}

i_threadGuard::~i_threadGuard()
{
	if (Thread.joinable())
	{
		Thread.join();
	}
}
