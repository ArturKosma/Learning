#include <iostream>
#include "World.h"

// This is Lightweight. Every tile has a pointer to 1MB of text. World consists of 10.000 of such tiles. Instead of keeping the text in every single tile
// we store intristic instances of each tile in the World, and the Tile matrix consists of pointers to those intristic instances. Thanks to
// that our program weights only ~3MB in the RAM.

int main()
{
	World MyWorld;

	std::cout << MyWorld.GetTile(0, 0)->GetTileName() << std::endl;
	std::cout << MyWorld.GetTile(50, 50)->GetTileName() << std::endl;
	std::cout << MyWorld.GetTile(13, 66)->GetTileName() << std::endl;

	getchar();
}
