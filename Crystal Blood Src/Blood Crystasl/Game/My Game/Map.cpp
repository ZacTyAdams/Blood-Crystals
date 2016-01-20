#include "Map.h"
#include <string.h>
#include "gamedefines.h"
#include <iostream>
#include <time.h>


map::map() { //constructor currently does nothing
	//playersUnits[0] = 0;
	//playersUnits[1] = 0;
	selectTarget = false;
	srand(time(NULL));
	baseDamage[0][0] = .55;
	baseDamage[1][0] = .45;
	baseDamage[2][0] = .05;
	baseDamage[0][1] = .65;
	baseDamage[1][1] = .55;
	baseDamage[2][1] = .55;
	baseDamage[0][2] = .75;
	baseDamage[1][2] = .70;
	baseDamage[2][2] = .55;
	
}

//prints out the tiles terrain id z
int map::printTerrain(int x, int y)
{
	return mapTerrain[x][y];
}

//checks current spot for unit eventually add for player as well 
bool map::isUnit(int xpos, int ypos) { //checks the unitmap for if it contains a unit
	if (xpos < 0 || xpos > 14 || ypos < 0 || ypos > 15) return false;
	else if (unitMap[xpos][ypos] != nullptr) return true;
	else return false;
}

int map::tileTerrain(int x, int y){ //returns the terrain located at a certain cell z
	return mapTerrain[x][y];
}

void map::getUnitInfo(int x, int y){ //prints the unit info located at a certain cell z
	
	char txt[50];

	if (isUnit(x, y)){
		int h, t, id, play;
		h = unitMap[x][y]->printHealth();
		t = unitMap[x][y]->printTerrain();
		id = unitMap[x][y]->printType();
		play = unitMap[x][y]->returnPlayer();

		sprintf_s(txt, "health: %d, terrain: %d, type: %d, player: %d", h, t, id, play);
		MessageBox(0, txt, "Current unit", 0);
	}
	else{
		sprintf_s(txt, "no unit was found");
		MessageBox(0, txt, "Current unit", 0);
	}

}

//int map::checkWin()
//{
	//if (playersUnits[0] == 0)
	//{
		//return 1;
	//}
	//else if (playersUnits[1] == 0)
	//{
		//return 2;
	//}
	//else
	//{
		//return 0;
	//}
//}

int map::unitPlayerGrabber(int x, int y){
	if (isUnit(x, y)){
		return unitMap[x][y]->returnPlayer();
	}
	else
		return 0;
}

bool map::checkTarget(int x, int y, int play){ //checks to see if any attack targets are in range z
	bool target[4];

	for (int i = 0; i <= 3; i++){
		target[i] = false;
	}

	if (isUnit((x - 1), y) && unitMap[x-1][y]->returnPlayer() != play){ //left
		target[0] = true;
	}
	if (isUnit(x, (y - 1)) && unitMap[x][y-1]->returnPlayer() != play){ //up
		target[1] = true;
	}
	if (isUnit((x + 1), y) && unitMap[x+1][y]->returnPlayer() != play){ //right
		target[2] = true;
	}
	if (isUnit(x, (y + 1)) && unitMap[x][y+1]->returnPlayer() != play){ //down
		target[3] = true;
	}


	if (target[0] || target[1] || target[2] || target[3])
	{
		return true;
	}
	else
	{
		return false;
	}
}

//check if clicked position is within range z
bool map::checkDistance(int x, int y, int c, int d) 
{
	int check = unitMap[c][d]->returnMoveDistance();
	if (x < (c + check) && y < (d + check)) //might need to be less/equal to
		return true;
	else
		return false;

}

bool map::attackUnit(int x, int y, int ax, int ay){ //function that handles unit tartgeting an attacking z
	int xtar, ytar, aid, tid, ater, tter, ahp, dhp, dtr, atr;
	double damageA, damageB;
	char txt[50];
	bool target[4];

	aid = unitMap[ax][ay]->printType();
	ater = unitMap[ax][ay]->printTerrain();
	ahp = unitMap[ax][ay]->printHealth();

	
	
	for (int i = 0; i <= 3; i++){
		target[i] = false;
	}

	if (isUnit((ax - 1), ay)){ //left
		if (x == (ax - 1) && y == ay){ //ax and ay is attacker position
			xtar = x;
			ytar = y;
			target[0] = true;
		}
	}
	if (isUnit(ax, (ay - 1))){ //up
		if (x == ax && y == (ay - 1)){
			xtar = x;
			ytar = y;
			target[1] = true;
		}
	}
	if (isUnit((ax + 1), ay)){ //right
		if (x == (ax+1) && y == ay){
			xtar = x;
			ytar = y;
			target[2] = true;
		}
	}
	if (isUnit(ax, (ay + 1))){ //down
		if (x == ax && y == (ay+1)){
			xtar = x;
			ytar = y;
			target[3] = true;
		}
	}


	if (target[0] || target[1] || target[2] || target[3])
	{
	xtar = x;
	ytar = y;
		
	tid = unitMap[xtar][ytar]->printType();
	tter = unitMap[xtar][ytar]->printTerrain();
	dhp = unitMap[xtar][ytar]->printHealth();

	switch (tter){
	case 205: //mountains
		dtr = 3;
		break;
	case 259: //forest
		dtr = 2;
		break;
	case 199: //plains
		dtr = 1;
		break;
	case 193: //city
		dtr = 4;
		break;
	case 196: //city
		dtr = 4;
		break;
	case 226: //roads
		dtr = 0;
		break;
	}


	damageA = (((baseDamage[tid-1][aid-1] + (.01*(rand() % 6))) * ahp) -  ((double)(dtr * .04) * dhp));
	if (damageA < 0){
		damageA = 0;
	}

	unitMap[xtar][ytar]->takeDamage(damageA);
 
	//sprintf_s(txt, "calc damage: %lf", damage);
	//MessageBox(0, txt, "attack func", 0);


	if (isUnit(xtar, ytar) && unitMap[xtar][ytar]->health <= 0){ //destroys unit when health reaches 0 z
		unitMap[xtar][ytar] = nullptr;
		return true;
	}
	else 
		return false;
		
	}
}

//selects unit
void map::selectUnit(int xpos, int ypos) { //selects a unit (check if spot is a unit first or it might select a nullptr)
	int i;
	int j;
	selected = unitMap[xpos][ypos];
	xselected = xpos;
	yselected = ypos;

	//prints where unit can move
	if (selected->returnPlayer() == 1) {
		for (i = 0; i < selected->returnMoveDistance() + 1; i++) { //code for drawing moveable locations
			for (j = selected->returnMoveDistance() - i; j > -1; j--) {
				if (xpos - i >= 0) {
					if (ypos - j >= 0 && !isUnit(xpos - i, ypos - j)) {
						drawMap[xpos - i][ypos - j] = tileMap[xpos - i][ypos - j].red;
					}
					if (ypos + j < 10 && !isUnit(xpos - i, ypos + j)) {
						drawMap[xpos - i][ypos + j] = tileMap[xpos - i][ypos + j].red;
					}
				}
				if (xpos + i < 15) {
					if (ypos - j >= 0 && !isUnit(xpos + i, ypos - j)) {
						drawMap[xpos + i][ypos - j] = tileMap[xpos + i][ypos - j].red;
					}
					if (ypos + j < 10 && !isUnit(xpos + i, ypos + j)) {
						drawMap[xpos + i][ypos + j] = tileMap[xpos + i][ypos + j].red;
					}
				}
			}
		}
	}
	else if (selected->returnPlayer() == 2) { //code for drawing move location
		for (i = 0; i < selected->returnMoveDistance() + 1; i++) {
			for (j = selected->returnMoveDistance() - i; j > -1; j--) {
				if (xpos - i >= 0) {
					if (ypos - j >= 0 && !isUnit(xpos - i, ypos - j)) {
						drawMap[xpos - i][ypos - j] = tileMap[xpos - i][ypos - j].blue;
					}
					if (ypos + j < 10 && !isUnit(xpos - i, ypos + j)) {
						drawMap[xpos - i][ypos + j] = tileMap[xpos - i][ypos + j].blue;
					}
				}
				if (xpos + i < 15) {
					if (ypos - j >= 0 && !isUnit(xpos + i, ypos - j)) {
						drawMap[xpos + i][ypos - j] = tileMap[xpos + i][ypos - j].blue;
					}
					if (ypos + j < 10 && !isUnit(xpos + i, ypos + j)) {
						drawMap[xpos + i][ypos + j] = tileMap[xpos + i][ypos + j].blue;
					}
				}
			}
		}
	}

	//These are used to see if it found a unit in eithe on of the categories
	bool foundUnitU = false; //Up
	bool foundUnitD = false; //Down
	bool foundUnitL = false; //Left
	bool foundUnitR = false; //Right
	bool foundUnitUL = false; //Top Left Corner
	bool foundUnitUR = false; //Top Right
	bool foundUnitDL = false; //Down Left
	bool foundUnitDR = false; //Down Right

	//Kinda hard to explain... Just know that it covers all the cases
	//where a unit shouldn't be able to move and makes sure it can't
	//I'll do my best to explain -Austin

	//The code 
	for (i = 1; i < selected->returnMoveDistance() + 1; i++) {
		if (xpos + i < 15) { 
			if (isUnit(xpos + i, ypos) || foundUnitR) { //if it found a unit to the right it remove the ability to move right
				drawMap[xpos + i][ypos] = tileMap[xpos + i][ypos].tile; 
				foundUnitR = true; //Set to true so once found every tile beyond is removed
			}
			if (ypos - i >= 0) {
				if (isUnit(xpos + i, ypos - i)) foundUnitUR = true; //found a unit to the upper right
			}
			if (ypos + i < 15 ) {
				if (isUnit(xpos + i, ypos + i)) foundUnitDR = true; //found a unit to the lower right
			}
		}
		if (xpos - i >= 0) {
			if (isUnit(xpos - i, ypos) || foundUnitL) { //Same as about but for the left
				drawMap[xpos - i][ypos] = tileMap[xpos - i][ypos].tile;
				foundUnitL = true;
			}
			if (ypos - i >= 0) {
				if (isUnit(xpos - i, ypos - i)) foundUnitUL = true; //found a unit to the upper left
			}
			if (ypos + i < 15) {
				if (isUnit(xpos - i, ypos + i)) foundUnitDL = true; //found a unit to the lower left
			}
		}
		if (ypos + i < 15) {
			if (isUnit(xpos, ypos + i) || foundUnitD) { //for below
				drawMap[xpos][ypos + i] = tileMap[xpos][ypos + i].tile;
				foundUnitD = true;
			}
		}
		if (ypos - i >= 0) {
			if (isUnit(xpos, ypos - i) || foundUnitU) { //for above
				drawMap[xpos][ypos - i] = tileMap[xpos][ypos - i].tile;
				foundUnitU = true;
			}
		}
		
		//This are for each of the case in the corners
 		for (j = 1; j < selected->returnMoveDistance() + 1; j++) {
			if (foundUnitU && foundUnitR && xpos + j < 15 && ypos - i >= 0) { //this stops all tiles in the upper right if both up and right are units
				drawMap[xpos + j][ypos - i] = tileMap[xpos + j][ypos - i].tile;
			}
			if (foundUnitU && foundUnitL && xpos - j >= 0 && ypos - i >= 0) { //this stops all tiles in the upper left if both up and right are units
				drawMap[xpos - j][ypos - i] = tileMap[xpos - j][ypos - i].tile;
			}
			if (foundUnitD && foundUnitR && xpos + j < 15 && ypos + i < 15) { //this stops all tiles in the lower right if both up and right are units
				drawMap[xpos + j][ypos + i] = tileMap[xpos + j][ypos + i].tile;
			}
			if (foundUnitD && foundUnitL && xpos - j >= 0 && ypos + i < 15) { //this stops all tiles in the lower left if both up and right are units
				drawMap[xpos - j][ypos + i] = tileMap[xpos - j][ypos + i].tile;
			}

			//is the draws up and down
			if (foundUnitUL && foundUnitU && xpos - j >= 0 && ypos - i >= 0) { //this stops upper left tiles if the upper left corner and up are units
				drawMap[xpos - j][ypos - i] = tileMap[xpos - j][ypos - i].tile;
			}
			if (foundUnitUR && foundUnitU && xpos + j <= 15 && ypos - i >= 0) { //this stops upper right tiles if the upper right corner and up are units
				drawMap[xpos + j][ypos - i] = tileMap[xpos + j][ypos - i].tile;
			}
			if (foundUnitDL && foundUnitD && xpos - j >= 0 && ypos + i < 15) { //this stops lower left tiles if lower left corner and down are units
				drawMap[xpos - j][ypos + i] = tileMap[xpos - j][ypos + i].tile;
			}
			if (foundUnitDR && foundUnitD && xpos + j < 15 && ypos + i < 15) { //this stops lower right tiles if lower right corner and down are units
				drawMap[xpos + j][ypos + i] = tileMap[xpos + j][ypos + i].tile;
			}

			//Same as about but draws left and right (above misses a few tiles)
			if (foundUnitUL && foundUnitL && xpos - i >= 0 && ypos - j >= 0) {
				drawMap[xpos - i][ypos - j] = tileMap[xpos - i][ypos - j].tile;
			}
			if (foundUnitUR && foundUnitR && xpos + i <= 15 && ypos - j >= 0) {
				drawMap[xpos + i][ypos - j] = tileMap[xpos + i][ypos - j].tile;
			}
			if (foundUnitDL && foundUnitL && xpos - i >= 0 && ypos + j < 15) {
				drawMap[xpos - i][ypos + j] = tileMap[xpos - i][ypos + j].tile;
			}
			if (foundUnitDR && foundUnitR && xpos + i < 15 && ypos + j < 15) {
				drawMap[xpos + i][ypos + j] = tileMap[xpos + i][ypos + j].tile;
			}

		}
	}
	drawMap[xpos][ypos] = tileMap[xpos][ypos].tile; //makes sure center is deselected
}

//checks if a unit is selected
bool map::isSelected() {
	if (selected != nullptr) return true;
	else return false;
}

//deselects unt (also returns tiles back to normal)
void map::deselectUnit() {
	selected = nullptr;
	int i;
	int j;
	for (i = 0; i < 15; i++) { //sets map back to normal
		for (j = 0; j < 10; j++) {
			drawMap[i][j] = tileMap[i][j].tile;
		}
	}
}

bool map::canMove(int xpos, int ypos) {
	std::cout << "can move" << endl;
	if (!isUnit(xpos, ypos) && drawMap[xpos][ypos] != tileMap[xpos][ypos].tile) {
		moveSelected = selected;
		deselectUnit();
		return true;
	}
	else if ((xselected == xpos && yselected == ypos) || !isUnit(xpos, ypos)) { //if the spot selected is the unit
		if (selectTarget){
			deselectUnit();
		}
		//checkTarget(xpos, ypos);
		if (!selectTarget){ //just deselects
			deselectUnit();			
		}
		return false;
	}
	else {
		deselectUnit(); //deselects old unit
		selectUnit(xpos, ypos); //selects new unit
		return false;
	}
}

//moves selected unit
void map::moveUnit(int xpos, int ypos) {
	unitMap[xselected][yselected] = nullptr; //sets old spot to null
	unitMap[xpos][ypos] = moveSelected; //moves unit to new spot
	unitMap[xpos][ypos]->getTerrain(mapTerrain[xpos][ypos]); //updates unit's current resting terrain z
	deselectUnit(); //deselects unit
}

void map::endTurn() {
}

int map::checkHome(int x, int y, int c, int d){
	char pos[50];
	if (isUnit(c, d)){
		if (x == 0 && y == 4 && unitPlayerGrabber(c, d) == 1){
			unitMap[c][d]->isHome = true;
			return unitMap[c][d]->isHome;
		}
		else if (x == 14 && y == 4 && unitPlayerGrabber(c, d) == 2){
			unitMap[c][d]->isHome = true;
			return unitMap[c][d]->isHome;
		}
		else
		{
			unitMap[c][d]->isHome = false;
			return unitMap[c][d]->isHome;
		}
	}
	else
		return -5;
}

void map::makeMap1(int r) { //defines a map 1 (could implement same process for multiple maps

	base tiles;

	rounds = r; //t passes in the number of turns
	turn = 1;

	int i;
	int j;

	//This map with map TILES, each tile can be given properties to be stored.
	//To add properties go to mapTile.h and give it another variable. 
	//The declare function is called when the tiles are created which is base.cpp. There it creates all the tiles we need so when ever you call
	//base it will create tiles with these names. In this case I called the base class with the name tiles

	//Below they are placed exactly like they are when viewed
	mapTile tempMap[10][15] = {
		{ tiles.mountain3, tiles.mountain2, tiles.mountain0, tiles.grass1, tiles.grass0, tiles.grass0, tiles.grass0, tiles.road6, tiles.grass0, tiles.grass0, tiles.grass0,  tiles.grass0, tiles.mountain0, tiles.mountain2, tiles.mountain3},
		{ tiles.mountain2, tiles.mountain0, tiles.grass1,    tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.road4,  tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0,    tiles.mountain0, tiles.mountain2},
		{ tiles.mountain0, tiles.grass1,    tiles.grass0,    tiles.tree0,  tiles.tree1,  tiles.grass1, tiles.grass0, tiles.road4,  tiles.grass0, tiles.grass0, tiles.tree0,  tiles.grass1, tiles.grass0,    tiles.grass0,    tiles.mountain0},
		{ tiles.grass0,    tiles.grass0,    tiles.grass0,    tiles.grass0, tiles.grass0, tiles.grass0, tiles.tree0,  tiles.road5,  tiles.grass0, tiles.tree0,  tiles.grass1, tiles.grass0, tiles.grass0,    tiles.grass0,    tiles.grass0},
		{ tiles.redBase,   tiles.road2,     tiles.road0,     tiles.road0,  tiles.road0,  tiles.road0,  tiles.road0,  tiles.road10, tiles.road0,  tiles.road0,  tiles.road0,  tiles.road0,  tiles.road0,     tiles.road0,     tiles.blueBase},
		{ tiles.grass0,    tiles.grass0,    tiles.grass0,    tiles.grass0, tiles.tree0,  tiles.grass1, tiles.grass0, tiles.road4,  tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0,    tiles.grass0,    tiles.grass0},
		{ tiles.grass0,    tiles.grass0,    tiles.grass0,    tiles.grass0, tiles.grass0, tiles.tree0,  tiles.grass1, tiles.road4,  tiles.grass0, tiles.tree0,  tiles.tree1,  tiles.grass1, tiles.grass0,    tiles.grass0,    tiles.grass0},
		{ tiles.mountain1, tiles.grass1,    tiles.grass0,    tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.road4,  tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0,    tiles.grass0,    tiles.mountain1},
		{ tiles.mountain3, tiles.mountain1, tiles.grass1,    tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.road4,  tiles.grass0, tiles.grass0, tiles.grass0, tiles.tree0,  tiles.grass1,    tiles.mountain1, tiles.mountain3},
		{ tiles.mountain2, tiles.mountain2, tiles.mountain0, tiles.grass1, tiles.grass0, tiles.grass0, tiles.grass0, tiles.road8,  tiles.grass0, tiles.grass0, tiles.grass0, tiles.grass0, tiles.mountain0, tiles.mountain2, tiles.mountain2}
	};

	for (i = 0; i < 15; i++) {
		for (j = 0; j < 10; j++) {
			tileMap[i][j] = tempMap[j][i]; //saves each tile from tempMap to the actual map
			unitMap[i][j] = nullptr; //currently don't have units
			drawMap[i][j] = tileMap[i][j].tile; //this is what needs to be drawn on to the map
			mapTerrain[i][j] = tileMap[i][j].returnTerrain(); //each map tile has a terrain this saves the current terrain to the terrain map
		}
	}
	moveSelected = nullptr;
}