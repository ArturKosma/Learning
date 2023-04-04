#include <iostream>
#include <array>
#include "SpecificCommand.h"

// Lacks redo but it's similar to implement.
// This is an example of the Command pattern implementation. We add new commands by typing the name of the command. We can
// undo the command at any time. Adding new command means executing it. After we undo, any new command will overwrite 
// all previously undoed commands.
// This should also handle the case in which adding more command than there's space in history for, makes the whole list shift down
// overwriting the oldest command. For perfect implementation the history of commands should be resizable.

int main()
{
	// {} initialises with value (0 which also means nullptr).
	std::array<BasicCommand*, 10> commandHistory {};
	auto currentCommand = commandHistory.begin();

	// Gameloop.
	while (true)
	{
		std::cout << "'a' to add new command." << std::endl;
		std::cout << "'b' to undo command." << std::endl;

		char input;
		std::cin >> input;

		// Clear input buffer.
		std::cin.clear();
		std::cin.ignore(STREAM_LIMIT, '\n');

		switch (input)
		{
		case 97: 
		{
			currentCommand++;

			// Erase all future commands since we've added a new one.
			for (auto it = currentCommand; it != commandHistory.end(); ++it)
			{
				delete *it;
				*it = nullptr;
			}

			*currentCommand = new SpecificCommand();
			(*currentCommand)->Execute();
			break;
		}
		case 98:
		{
			if (currentCommand != commandHistory.begin())
			{
				(*currentCommand)->Undo();
				currentCommand--;
			}
			else
			{
				printf("Nothing else to undo.\n");
			}
			break;
		}
		default:
		{
			break;
		}
		}

		if (*currentCommand != nullptr)
		{
			printf("Last command name: %s\n", (*currentCommand)->CommandName.c_str());
			printf("Commands so far:\n");
			for (size_t i = 1; i < commandHistory.size(); ++i)
			{
				std::string commandName = commandHistory[i] == nullptr ? "-" : commandHistory[i]->CommandName;
				printf("%u. %s\n", i, commandName.c_str());
			}
		}
	}
}
