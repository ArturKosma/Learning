#include "BasicCommand.h"
#include <iostream>

BasicCommand::BasicCommand()
{
	std::cout << "Name: ";
	std::cin >> CommandName;
	std::cout << std::endl;

	// Clear input buffer.
	std::cin.clear();
	std::cin.ignore(STREAM_LIMIT, '\n');
}

BasicCommand::~BasicCommand()
{

}
