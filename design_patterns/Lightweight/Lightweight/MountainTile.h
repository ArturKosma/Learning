#pragma once
#include "Tile.h"
class MountainTile : public Tile
{
public:
	MountainTile();
	~MountainTile();

	virtual const char* GetTileHeavyData() const override;


	virtual std::string GetTileName() const override;

private:

	// 1MB of data.
	const char* SomeVeryHeavyData = new char[1024 * 1024];

};

