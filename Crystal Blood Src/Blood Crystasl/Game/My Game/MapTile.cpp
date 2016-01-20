#include "MapTile.h"

void mapTile::declare(int t, int r, int b, int terrainType) {
	tile = t; //sets tile to t (regular tile)
	red = r; //sets red to r (red tile for player 1)
	blue = b; //sets blue to b (blue tile for player 2)
	terrain = terrainType;
}

int mapTile::returnTerrain() {
	return terrain; //stores the terrain type which will have stats
}