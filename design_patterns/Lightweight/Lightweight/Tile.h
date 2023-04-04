#pragma once
#include <string>

class Tile
{
public:
	Tile();
	~Tile();

	virtual const char* GetTileHeavyData() const = 0;
	virtual std::string GetTileName() const = 0;
};

