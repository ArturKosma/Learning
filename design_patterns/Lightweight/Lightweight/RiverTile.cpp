#include "RiverTile.h"



RiverTile::RiverTile()
{
}


RiverTile::~RiverTile()
{
}

const char* RiverTile::GetTileHeavyData() const
{
	return MyGodBigData;
}

std::string RiverTile::GetTileName() const
{
	return "RiverTile";
}
