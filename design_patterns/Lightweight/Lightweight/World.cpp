#include "World.h"
#include <stdlib.h>
#include <time.h>


World::World()
{
	srand(time(NULL));

	for (size_t i = 0; i < 100; ++i)
	{
		for (size_t n = 0; n < 100; ++n)
		{
			int tileType = rand() % 3;

			switch (tileType)
			{
			case 0:
			{
				WorldTiles[i][n] = &ForestTileModel;
				break;
			}
			case 1:
			{
				WorldTiles[i][n] = &MountainTileModel;
				break;
			}
			case 2:
			{
				WorldTiles[i][n] = &RiverTileModel;
				break;
			}
			default:
			{
				break;
			}
			}

		}
	}
}


World::~World()
{
}

Tile* World::GetTile(int InX, int InY) const
{
	return WorldTiles[InX][InY];
}
