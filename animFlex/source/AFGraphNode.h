#pragma once

#include "AFGraphNodeRegistry.h"

class AFGraphNode
{
public:
	virtual ~AFGraphNode();

	virtual void Evaluate(float deltaTime) = 0;
};