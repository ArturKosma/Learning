#pragma once
#include "BasicCommand.h"

class SpecificCommand : public BasicCommand
{
public:

	SpecificCommand();
	~SpecificCommand();

	virtual void Execute() override;

	virtual void Undo() override;

};

