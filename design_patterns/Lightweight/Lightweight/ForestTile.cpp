#include "ForestTile.h"



ForestTile::ForestTile()
{
}


ForestTile::~ForestTile()
{
}

const char* ForestTile::GetTileHeavyData() const
{
	return SomeHeavyData;
}

std::string ForestTile::GetTileName() const
{
	return "ForestTile";
}
