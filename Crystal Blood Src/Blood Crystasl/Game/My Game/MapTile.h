//Base declares types of maptiles that are used in the Map.cpp class to create a map
//currently hardcoded since if it works don't break it

class mapTile {
	friend class map; //allows map to edit
private:
	int tile, red, blue; //saves 3 tile sprites
	int terrain;
public:
	void declare(int t, int r, int b, int terrainType); //called when declaring what it will be
	int returnTerrain();
};