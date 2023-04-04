#include "MountainTile.h"



MountainTile::MountainTile()
{
}


MountainTile::~MountainTile()
{
}

const char* MountainTile::GetTileHeavyData() const
{
	return SomeVeryHeavyData;
}

std::string MountainTile::GetTileName() const
{
	return "MountainTile";
}
