#include "SpecificCommand.h"
#include <iostream>

SpecificCommand::SpecificCommand()
{

}

SpecificCommand::~SpecificCommand()
{

}

void SpecificCommand::Execute()
{
	std::cout << "executing command " << CommandName << std::endl;
}

void SpecificCommand::Undo()
{
	std::cout << "undoing command " << CommandName << std::endl;
}
