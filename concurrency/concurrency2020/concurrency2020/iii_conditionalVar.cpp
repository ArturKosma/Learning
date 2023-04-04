#include "pch.h"
#include "iii_conditionalVar.h"
#include <iostream>


void iii_conditionalVar::main()
{
	std::unique_lock<std::mutex> ul(mtx);
	std::thread th(&iii_conditionalVar::separateThread, this);
	th.detach();



	var.wait(ul);

	std::cout << "MAM TE MOC" << std::endl;
}

void iii_conditionalVar::separateThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	var.notify_one();
}

iii_conditionalVar::iii_conditionalVar()
{
}


iii_conditionalVar::~iii_conditionalVar()
{
}
