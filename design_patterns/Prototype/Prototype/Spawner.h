#pragma once
#include "Monster.h"

template<typename MonsterType>
class Spawner
{
public:

	Spawner(MonsterType* InPrototype) : Prototype(InPrototype)
	{

	}

	~Spawner()
	{

	}

	Monster* Spawn() const
	{
		return Prototype->Clone();
	}

private:

	MonsterType* Prototype;
};

