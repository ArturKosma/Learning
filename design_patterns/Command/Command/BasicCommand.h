#pragma once
#include <string>

#ifndef STREAM_LIMIT
#define STREAM_LIMIT std::numeric_limits<std::streamsize>::max()
#endif

class BasicCommand
{
public:

	BasicCommand();
	~BasicCommand();

	virtual void Execute() = 0;
	virtual void Undo() = 0;

	std::string CommandName;
};

