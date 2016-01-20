#include "unit.h"
#include "gamedefines.h"

unit::unit() {
	health = 100;
	unitType = 0;
	Moved = false; //used to identify if the unit has moved or not z
	player = 0; //used to identify the unit's player id z
	moveDistance = 0;
	isHome = false;
	canCap = false;
	home = -1;
}

bool unit::returnCap(){
	return canCap;
}

bool unit::hasMoved(){
	return Moved;
}

void unit::changeMoved(){
	if (Moved)
	{
		Moved = false;
	}
	else
	{
		Moved = true;
	}
}

int unit::printHealth(){ //returns current health of unit z
	return health;
}

int unit::printType(){ //returns the unit type of unit z
	return unitType;
}

void unit::healing(){
	if (isHome){
		if (health <= 75)
			health = health + 25;
		else
			health = health + (100 - health);
	}
}

bool unit::setHome(){ //grabs location from map class z
	if (isHome)
	{
		isHome = false;
	}
	else
	{
		isHome = true;
	}
	return isHome;
}

void unit::getTerrain(int t){ //grabs terrain from map class z
	terrain = t;
}

int unit::printTerrain(){ //returns terrain unit is in z
	return terrain;
}

void unit::takeDamage(int dam){
	health = health - dam;
}

int unit::returnMoveDistance() { //returns moveDistance
	switch (terrain){
	case 205: //mountains
		return moveDistance - 1;
		break;
	case 259: //forest
		return moveDistance - 1;
		break;
	case 199: //plains
		return moveDistance;
		break;
	case 193: //city
		return moveDistance + 1;
		break;
	case 196: //city
		return moveDistance + 1;
		break;
	case 226: //roads
		return moveDistance + 1;
		break;
	}
}

int unit::returnPlayer() { //returns player id for unit z
	return player;
}

void unit::makeSoldier(int p) { //soldier (send it player to create a soldier)
	moveDistance = 2;
	player = p;
	int i;
	int j;
	unitType = 1; //sets the unit type to be used in damage calculation z
	if (player == 2) {
		home = 14;
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = BSIDLE0 + (i * 4) + j;
			}
		}
	}
	else if (player == 1){
		home = 0;
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = RSIDLE0 + (i * 4) + j;
			}
		}
 	}
}

void unit::makeHeavySoldier(int p) { //heavy soldier (send it player to create a soldier)
	moveDistance = 2;
	player = p;
	unitType = 2;
	int i;
	int j;
	if (player == 2) {
		home = 14;
		for(i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = BHIDLE0 + (i * 4) + j;
			}
		}
	}
	else if (player == 1){
		home = 0;
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = RHIDLE0 + (i * 4) + j;
			}
		}
	}
}

void unit::makeTank(int p) { //tank (send it player to create a soldier)
	moveDistance = 3;
	player = p;
	unitType = 3;
	int i;
	int j;
	if (player == 2) {
		home = 14;
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = BTIDLE0 + (i * 4) + j;
			}
		}
	}
	else if (player == 1){
		home = 0;
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 4; j++) {
				frames[i][j] = RTIDLE0 + (i * 4) + j;
			}
		}
	}
}

