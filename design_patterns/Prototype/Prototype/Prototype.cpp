#include <iostream>
#include "Ghost.h"
#include "Spawner.h"

// Prototype is for less boilerplate code.
// You don't want a unique spawner class for every monster.
// You can use existing monster to create copies of them.
// That's all.

int main()
{
	Ghost* protGhost = new Ghost(445, 666, 0.6f);
	Spawner<Ghost> ghostSpawner = Spawner<Ghost>(protGhost);

	Monster* spawnedGhost = ghostSpawner.Spawn();

	std::cout << spawnedGhost->GetDebugData() << std::endl;
}
