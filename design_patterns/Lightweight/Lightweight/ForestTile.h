#pragma once
#include "Tile.h"
class ForestTile : public Tile
{
public:
	ForestTile();
	~ForestTile();


	virtual const char* GetTileHeavyData() const override;


	virtual std::string GetTileName() const override;

private:

	// 1MB of data.
	const char* SomeHeavyData = new char[1024 * 1024];

};

