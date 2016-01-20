#include "Base.h"

class map {
private:
	friend class unit;
	unit* soldier;
	int mapTerrain[15][10]; //these values will be used in damage and movement calculation z
	int rounds;
	int turn; //matches the player whos turn it is
	double baseDamage[3][3];

public:
	map();
	mapTile tileMap[15][10];
	void makeMap1(int r);
	unit* unitMap[15][10];
	int drawMap[15][10];
	unit* selected;
	unit* moveSelected;
	bool isUnit(int xpos, int ypos);
	void selectUnit(int xpos, int ypos);
	bool isSelected();
	int xselected;
	int yselected;
	bool canMove(int xpos, int ypos); //if movement can be move returns true else false
	void moveUnit(int xpos, int ypos);
	void deselectUnit();
	void endTurn();
	void getUnitInfo(int x, int y);// pulls unit infoz
	bool attackUnit(int x, int y, int ax, int ay); //used to attack units z
	int checkWin();
	int tileTerrain(int x, int y);
	bool checkTarget(int x, int y, int play);
	bool selectTarget;
	int printTerrain(int x, int y);
	bool checkDistance(int x, int y, int c, int d);
	int unitPlayerGrabber(int x, int y);
	void setCap(int x, int y);
	int checkHome(int x, int y, int c, int d);

};