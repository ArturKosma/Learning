#pragma once

#include "ForestTile.h"
#include "MountainTile.h"
#include "RiverTile.h"

class World
{
public:
	World();
	~World();

	Tile* GetTile(int InX, int InY) const;

private:

	Tile* WorldTiles[100][100];

	ForestTile ForestTileModel;
	MountainTile MountainTileModel;
	RiverTile RiverTileModel;
};

