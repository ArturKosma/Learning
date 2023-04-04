#pragma once
#include "Tile.h"
class RiverTile : public Tile
{
public:
	RiverTile();
	~RiverTile();

	virtual const char* GetTileHeavyData() const override;


	virtual std::string GetTileName() const override;

private:

	// 1MB of data.
	const char* MyGodBigData = new char[1024 * 1024];

};

