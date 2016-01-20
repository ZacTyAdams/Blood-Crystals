/// \file MyGame.cpp  
/// \brief Main file for your game.

/// \mainpage Ball and Spring Toy
/// This code was written by Ian Parberry to accompany his book
/// "Introduction to Game Physics with Box2D", published by CRC Press in 2013.
/// 
/// Having been introduced to the theory of Verlet integration and Gauss-
/// Seidel relaxation in Chapter 2, we now examine code for a Ball and Spring
/// Toy, which lets you play with a body made up of a bunch
/// of balls connected by constraints. The constraints can be either stick-like
/// (with no springiness), or spring-like (with lots of springiness). You can
/// cycle through various types of body and you can apply impulses to them
/// to see how they react. The toy starts with the body in the air, from which
/// position it drops under gravity to rest at the bottom of the screen.
/// 
/// The controls are very basic. The space bar applies an impulse in a
/// random direction, the Enter key restarts with the current body type, and
/// the Back key advances to the next body type. The Escape key, as always,
/// quits.
/// 
/// This is a toy, not a game, because there is no concept of winning or losing.

#include "gamedefines.h"
#include "SndList.h"
#include "RenderWorld.h"
#include "ObjectWorld.h"
#include "Map.h"

POINT pt;
POINT poi;

extern HWND g_HwndApp; 
extern HINSTANCE g_hInstance;

void PrintButton();
int WinLoc(long, long, int, int);
int m_nSoundInstance;
int a = 14;
int b = 5;
int c = 0;
int d = 0; //used to determin the attack target cells z
int turn = 1;
int timer = 0;
int aniTimer = 0; //animation timer
int cLevel = 0; //used as the levels between activities when clicking
int play;
int playerUnits[2];

//playerUnits[2];

int targetBase = 0;
int baseHealth = 200;

bool canAttack = false;
bool canCapture = false;
bool bloodFinished = true;
bool bloodFinished2 = true;
bool bloodFinished3 = true;
bool bloodFinished4 = true;
bool bloodFinished5 = true;
bool bloodFinished6 = true;
bool gameWinner = false;
bool exAnimation = false; //if true draws the explosion
bool shAnimation = false; //shooting animation

Vector3 explosion;
Vector3 attackee;

int winner = 0;
float movementSpeed = (float)4; //how fast it moves (if you want to change make it a multiple of 64)

map* startMap = new map; //makes a map (possible could make more maps)

Vector3 current; //if unit is moving where it currently is
Vector3 nextTile; //the next tile the unit moves to
Vector3 final; //last tile the unit has to land on
Vector3 blood;
Vector3 blood2;
Vector3 blood3;
Vector3 blood4;
Vector3 blood5;
int bloodTimer = 0;
int shootTimer = 0;
bool showCrosshair = false;
Vector3 blood6;
//globals
char g_szGameName[256]; ///< Name of this game.
bool splashScreen;
bool moveUnit;
unit* playerArray[6];

CTimer g_cTimer; ///< The game timer.
CSoundManager* g_pSoundManager; ///< The sound manager.

//Render and Object Worlds
CRenderWorld g_cRenderWorld; ///< The Render World.
CObjectWorld g_cObjectWorld; ///< The Object World.

//prototypes for Windows functions
int DefaultWinMain(HINSTANCE, HINSTANCE, LPSTR, int);
LRESULT CALLBACK DefaultWindowProc(HWND, UINT, WPARAM, LPARAM);

/// Start the game.


void BeginGame(){ 
  g_cTimer.StartLevelTimer(); //starting level now     
} //BeginGame

/// Initialize and start the game.

void addUnitsToMap() { //wanted to add it to map but kept getting no storage error so for now the unit is made in the main

	// player 2 blue
	unit* soldierHB = new unit; //credates a new unit
	soldierHB->makeHeavySoldier(2); // makes a heavy soldier for player 2
	startMap->unitMap[14][3] = soldierHB; //places player 2 heavy soldiers
	soldierHB->getTerrain(startMap->tileTerrain(14, 3)); //sets unit's current terrain z
	playerUnits[1]++; //stars unit in storage array z
	playerArray[0] = soldierHB;

	unit* soldierB = new unit;
	soldierB->makeSoldier(2); // makes a soldier for player 2
	startMap->unitMap[13][4] = soldierB; //places player 2 soldiers
	soldierB->getTerrain(startMap->tileTerrain(13, 4)); //sets unit's current terrain z
	playerUnits[1]++; //stars unit in storage array z
	playerArray[1] = soldierB;

	unit* tankB = new unit;
	tankB->makeTank(2); // makes a tank for player 2
	startMap->unitMap[14][5] = tankB; //places player 2 tank
	tankB->getTerrain(startMap->tileTerrain(14, 5)); //sets unit's current terrain z
	playerUnits[1]++; tankB; //stars unit in storage array z
	playerArray[2] = tankB;

	// player 1 red
	unit* soldierHR = new unit;
	soldierHR->makeHeavySoldier(1); // makes a heavy soldier for player 1
	startMap->unitMap[0][3] = soldierHR; //places player 1 heavy soldiers
	soldierHR->getTerrain(startMap->tileTerrain(0, 3)); //sets unit's current terrain z
	playerUnits[0]++; //stars unit in storage array z
	playerArray[3] = soldierHR;

	unit* soldierR = new unit;
	soldierR->makeSoldier(1); //makes a soldier of player 1
	startMap->unitMap[1][4] = soldierR; //places player 1 soldiers
	soldierR->getTerrain(startMap->tileTerrain(1, 4)); //sets unit's current terrain z
	playerUnits[0]++; //stars unit in storage array z
	playerArray[4] = soldierR;

	unit* tankR = new unit;
	tankR->makeTank(1); // makes a tank for play 1
	startMap->unitMap[0][5] = tankR; // places player 1 tank
	tankR->getTerrain(startMap->tileTerrain(0, 5)); //sets unit's current terrain z
	playerUnits[0]++; //stars unit in storage array z
	playerArray[5] = tankR;
}

void InitGame(){
  g_cRenderWorld.Initialize(g_hInstance, g_HwndApp); //bails if it fails
  g_cRenderWorld.LoadImages(); //load images from xml file list
  GetCapture();
  ShowCursor(TRUE);
  splashScreen = true;
  moveUnit = false;
  playerUnits[0] = 0;
  playerUnits[1] = 0;
  BeginGame(); //it starts...
  g_cRenderWorld.LoadSplash();
  g_cRenderWorld.DrawBackground();
  PrintButton();
  startMap->makeMap1(20); //makes map Map1
  addUnitsToMap(); //adds units to Map1
} //InitGame

/// Shut down game and release resources.

void EndGame(){
	g_pSoundManager->stop(MATCH);
	g_cRenderWorld.Release();
	SAFE_DELETE(g_pSoundManager); 
	ShowCursor(TRUE);
} //EndGame

void WinnerAnimation() {
	Vector3 winnerTitle;
	winnerTitle.x = 960 / 2;
	winnerTitle.y = 300;
	winnerTitle.z = -10;
	splashScreen = true;
	if (winner == 1) {
		g_cRenderWorld.draw(WIN1, winnerTitle);
	}
	if (winner == 2){
		g_cRenderWorld.draw(WIN2, winnerTitle);
	}
}

void PrintButton(){

	bloodTimer++;
	long x = 0;
	long y = 0;

	GetCursorPos(&poi);
	ScreenToClient(g_HwndApp, &poi);
	x = poi.x;
	y = poi.y;
	Vector3 splashButton;
	splashButton.x = 960 / 2;
	splashButton.y = 150;
	splashButton.z = 0;
	if (x >= (960 / 2) - 150 && x <= (960 / 2) + 150 && y >= 640 - 250 && y <= 640 - 150) {
		g_cRenderWorld.draw(START_HOVER, splashButton);
	}
	else {
		g_cRenderWorld.draw(START, splashButton);
	}

	splashButton.y = 500;
	splashButton.z = -10;
	g_cRenderWorld.draw(TITLE_NAME, splashButton);
	if (bloodFinished && bloodTimer < 10) {
		bloodFinished = false;
		blood.x = x;
		blood.y = 640 - y;
		blood.z = -11;
	}
	else if (bloodFinished2 && bloodTimer < 20 && bloodTimer >= 10) {
		bloodFinished2 = false;
		blood2.x = x;
		blood2.y = 640 - y;
		blood2.z = -12;
	}
	else if (bloodFinished3 && bloodTimer < 30 && bloodTimer >= 20) {
		bloodFinished3 = false;
		blood3.x = x;
		blood3.y = 640 - y;
		blood3.z = -13;
	}
	else if (bloodFinished4 && bloodTimer < 40 && bloodTimer >= 30) {
		bloodFinished4 = false;
		blood4.x = x;
		blood4.y = 640 - y;
		blood4.z = -14;
	}
	else if (bloodFinished5 && bloodTimer < 50 && bloodTimer >= 40) {
		bloodFinished5 = false;
		blood5.x = x;
		blood5.y = 640 - y;
		blood5.z = -15;
	}
	else if (bloodFinished6 && bloodTimer < 60 && bloodTimer >= 50) {
		bloodFinished6 = false;
		blood6.x = x;
		blood6.y = 640 - y;
		blood6.z = -16;
	}
	if (!bloodFinished) {
		g_cRenderWorld.draw(BLOOD, blood);
		blood.y -= 23;
	}
	if (!bloodFinished2) {
		g_cRenderWorld.draw(BLOOD, blood2);
		blood2.y -= 23;
	}
	if (!bloodFinished3) {
		g_cRenderWorld.draw(BLOOD, blood3);
		blood3.y -= 23;
	}
	if (!bloodFinished4) {
		g_cRenderWorld.draw(BLOOD, blood4);
		blood4.y -= 23;
	}
	if (!bloodFinished5) {
		g_cRenderWorld.draw(BLOOD, blood5);
		blood5.y -= 23;
	}
	if (!bloodFinished6) {
		g_cRenderWorld.draw(BLOOD, blood6);
		blood6.y -= 23;
	}
	if (blood.y < 0) bloodFinished = true;
	if (blood2.y < 0) bloodFinished2 = true;
	if (blood3.y < 0) bloodFinished3 = true;
	if (blood4.y < 0) bloodFinished4 = true;
	if (blood5.y < 0) bloodFinished5 = true;
	if (blood6.y < 0) bloodFinished6 = true;
	if (bloodTimer == 60) bloodTimer = 0;
}
//makes the explosion
void ExplosionAnimation(){
	exAnimation = true;
	g_cRenderWorld.draw(EX0 + (aniTimer / 2), explosion); 
	aniTimer++;
	if (aniTimer >= 46) {
		aniTimer = 0;
		exAnimation = false;
	}
}

void ShootingAnimation(){
	shAnimation = true;
	g_cRenderWorld.draw(ATTACK0 + (shootTimer / 10), explosion);
	shootTimer++;
	if (shootTimer >= 40) {
		shootTimer = 0;
		shAnimation = false;
	}
}

void Button(){
	long x = 0;
	long y = 0;

	GetCursorPos(&poi);
	ScreenToClient(g_HwndApp, &poi);
	x = poi.x;
	y = poi.y;

	Vector3 endButton;//end turn button
	endButton.x = 1056;
	endButton.y = 34;
	endButton.z = -2;
	if (x >= 960 && y >= 576){
		g_cRenderWorld.draw(ENDTURNHOVER, endButton);
	}
	else{
		g_cRenderWorld.draw(ENDTURN, endButton);
	}

	Vector3 attackButton;// attack button
	attackButton.x = 1056;
	attackButton.y = 500;
	attackButton.z = -2;
	if (x >= 960 && y >= 108 && y <= 169){
		g_cRenderWorld.draw(ATTACKHOVER, attackButton);
	}
	else if (startMap->checkTarget(c, d, play)){
		g_cRenderWorld.draw(ATTACKACTIVE, attackButton);
	}
	else{
		g_cRenderWorld.draw(ATTACK, attackButton);
	}

	Vector3 WaitButton;// wait button
	WaitButton.x = 1056;
	WaitButton.y = 400;
	WaitButton.z = -2;
	if (x >= 960 && y >= 208 && y <= 269){
		g_cRenderWorld.draw(WAITHOVER, WaitButton);
	}
	else if (startMap->isSelected()){
		g_cRenderWorld.draw(WAITACTIVE, WaitButton);
	}
	else{
		g_cRenderWorld.draw(WAIT, WaitButton);
	}

	Vector3 CapButton;// info button
	CapButton.x = 1056;
	CapButton.y = 300;
	CapButton.z = -2;
	if (x >= 960 && y >= 308 && y <= 369){
		g_cRenderWorld.draw(CAPTUREHOVER, CapButton);
	}
	else if (canCapture){
		g_cRenderWorld.draw(CAPTUREACTIVE, CapButton);
	}
	else{
		g_cRenderWorld.draw(CAPTURE, CapButton);
	}
}

void BCrystle(){
	Vector3 BC;// info button
	BC.x = 1056;
	BC.y = 170;
	BC.z = -2;

	if (timer < 10){
		g_cRenderWorld.draw(BLOODCRYSTLE0, BC);
		timer++;
	}
	else if (timer >= 10 && timer < 20){
		g_cRenderWorld.draw(BLOODCRYSTLE1, BC);
		timer++;
	}
	else if (timer >= 20 && timer < 30){
		g_cRenderWorld.draw(BLOODCRYSTLE2, BC);
		timer++;
	}
	else if (timer >= 30 && timer < 40){
		g_cRenderWorld.draw(BLOODCRYSTLE3, BC);
		timer++;
	}
	else if (timer >= 40 && timer < 50){
		g_cRenderWorld.draw(BLOODCRYSTLE4, BC);
		timer++;
	}
	else if (timer == 50){
		g_cRenderWorld.draw(BLOODCRYSTLE0, BC);
		timer = 0;
	}
}

/// Render a frame of animation.

void UnitManager(int x, int y){ //draws single unit on the screen z
	Vector3 solider;
	solider.x = (float)(64 * x - (64 / 2));
	solider.y = (float)(640 - 64 * (y - 1) - (64 / 2));
	solider.z = 0;
	//int sprites[4] = { IDLE0, IDLE1, IDLE2, IDLE3 };
	//g_cRenderWorld.drawAnimated(sprites, 300, 4, solider);
}

//this how the info pane is shown
//right now its just thrown together still waiting on other but it's a good start
void showInfoPane(int x, int y) {
	Vector3 infoPane;
	infoPane.y = 128;
	infoPane.z = -3;
	int terrainMod = 0; // interpreter from terrain id to mod
	int terr = startMap->printTerrain(a - 1, b - 1);

	switch (terr){
	case 205: //mountains
		terrainMod = 3;
		break;
	case 259: //forest
		terrainMod = 2;
		break;
	case 199: //plains
		terrainMod = 1;
		break;
	case 193: //city
		terrainMod = 4;
		break;
	case 196: //city
		terrainMod = 4;
		break;
	case 226: //roads
		terrainMod = 0;
		break;
	}

	Vector3 tile;
	tile.y = 160;
	tile.z = -4;
	if (x < 480) { //draws on left
		if (startMap->isUnit(x / 64, y / 64)) {
			//draws the two black boxes
			infoPane.x = 64 * 11 + 32;
			g_cRenderWorld.draw(INFOPANE, infoPane);
			tile.x = infoPane.x;
			g_cRenderWorld.draw(startMap->tileTerrain(x / 64, y / 64), tile);
			//draw terrain defense mod
			tile.x = infoPane.x - 24;
			tile.z = -4;
			tile.y = 112;
			g_cRenderWorld.draw(DEFENSE_ICON, tile); //draw shield
			//Defense mod
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(terrainMod, tile);

			tile.y = 160;

			infoPane.x = 64 * 13 + 32;
			g_cRenderWorld.draw(INFOPANE, infoPane);
			tile.x = infoPane.x;

			unit* temp = new unit;
			temp = startMap->unitMap[x / 64][y / 64]; //saves current unit to temp
			g_cRenderWorld.drawAnimated(temp->frames[0], 250, 4, tile); //prints temp with the idle frames
			int health = temp->printHealth(); //retrieves health from unit
			//health icon location
			tile.x = infoPane.x - 24;
			tile.z = -4;
			tile.y = 112;
			g_cRenderWorld.draw(HEALTH_ICON, tile); //draw heart
			//Amount of health
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(health / 10, tile);

			tile.x = infoPane.x - 24;
			tile.y = infoPane.y - (16 * 2) - 2;
			g_cRenderWorld.draw(MOVE_ICON, tile);
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(temp->returnMoveDistance(), tile);

		}
		else { //if there is no unit
			//draws just the info pane (currently terrain's have no info)
			infoPane.x = 64 * 13 + 32;
			g_cRenderWorld.draw(INFOPANE, infoPane);
			tile.x = infoPane.x;
			g_cRenderWorld.draw(startMap->tileTerrain(x / 64, y / 64), tile);

			//draw terrain defense mod
			tile.x = infoPane.x - 24;
			tile.z = -4;
			tile.y = 112;
			g_cRenderWorld.draw(DEFENSE_ICON, tile); //draw shield
			//Defense mod
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(terrainMod, tile);
		}
	}
	else if (x >= 480) { //draws on right
		//same as about but terrain is also ways on the right so takes slightly less code
		if (startMap->isUnit(x / 64, y / 64)) {
			//draws the unit info
			infoPane.x = 64 * 4 - 32;
			g_cRenderWorld.draw(INFOPANE, infoPane);
			tile.x = infoPane.x;
			unit* temp = new unit;
			temp = startMap->unitMap[x / 64][y / 64];
			g_cRenderWorld.drawAnimated(temp->frames[0], 250, 4, tile);
			int health = temp->printHealth();
			tile.x = infoPane.x - 24;
			tile.z = -4;
			tile.y = 112;
			g_cRenderWorld.draw(HEALTH_ICON, tile);
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(health / 10, tile);

			tile.x = infoPane.x - 24;
			tile.y = infoPane.y - (16 * 2) - 2;
			g_cRenderWorld.draw(MOVE_ICON, tile);
			tile.x = infoPane.x + 16;
			g_cRenderWorld.draw(temp->returnMoveDistance(), tile);
		}
		//draws the terrain info
		infoPane.x = 64 * 2 - 32;
		tile.x = infoPane.x;
		tile.y = 160;
		g_cRenderWorld.draw(INFOPANE, infoPane);
		g_cRenderWorld.draw(startMap->tileTerrain(x / 64, y / 64), tile);

		//draw terrain defense mod
		tile.x = infoPane.x - 24;
		tile.z = -4;
		tile.y = 112;
		g_cRenderWorld.draw(DEFENSE_ICON, tile); //draw shield
		//Defense mod
		tile.x = infoPane.x + 16;
		g_cRenderWorld.draw(terrainMod, tile);
	}
}

void SelectManager(){ //handles the auto position and locking for the cursor frame
	GetCursorPos(&poi);
	long x, y;
	int a1, b1;
	GetCursorPos(&poi);
	ScreenToClient(g_HwndApp, &poi);
	x = poi.x;
	y = poi.y;
	if (y > 630) y = 639;
	else if (y < 1) y = 1;
	if (x >(64 * 18) - 1) x = (64 * 18) - 1;
	else if (x< 1) x = 1;
	if (x >= 960) {
		a1 = (int)(x / 64) + 1;
		b1 = (int)(y / 64) + 1;

		Vector3 select;
		select.x = (float)x;
		select.y = (float)(640 - y);
		select.z = -100;
		g_cRenderWorld.draw(CURSOR_HAND, select);
	}
	else if(!moveUnit && x >= 0) {
		a1 = (int)(x / 64) + 1;
		b1 = (int)(y / 64) + 1;

		Vector3 select;
		select.x = (float)(64 * a1 - (32));
		select.y = (float)(640 - 64 * (b1 - 1) - (64/2));
		select.z = -1;
		if(showCrosshair) g_cRenderWorld.draw(CROSSHAIR, select);
		else g_cRenderWorld.draw(CURSOR, select);
		
		showInfoPane(x, y); //this is calls the info pan class
	}
}

void Move(){ //sets the (x,y) coordinate of the cursor pos z
	long x, y;

	GetCursorPos(&poi);
	ScreenToClient(g_HwndApp, &poi);
	x = poi.x;
	y = poi.y;

	a = (int)(x / 64) + 1;
	b = (int)(y / 64) + 1;

}

void unitMoveAlgorithm() { //this is the algorthim for how the unit move without collision
	unit* temp = new unit;
	temp = startMap->moveSelected;
	if (current == nextTile) { //means we made it to the next tile
		//always move x first unless there is a collision
		if (current.x < final.x) nextTile.x = current.x + 64; //so if x still isn't equal
		else if (current.x > final.x) nextTile.x = current.x - 64;
		else if (current.y < final.y) nextTile.y = current.y + 64; //if y stil isn't equal
		else if (current.y > final.y) nextTile.y = current.y - 64;
		else { //means equals final so it doesn't need to move anymore
			startMap->moveUnit((int)(((final.x - 32) / 64)), (int)((((640 - 32) - final.y) / 64))); //after its move puts unit back on Unit Map
			startMap->moveSelected = nullptr;
			moveUnit = false;
			if (temp->printType() == 3) g_pSoundManager->stop(TANK);
			else g_pSoundManager->stop(STEPS);
		}
	}

	//checks if the next tile is a unit (since it moves in x first we just have to make it move in the y direction)
	if (startMap->isUnit((int)(((nextTile.x - 32) / 64)), (int)((((640 - 32) - nextTile.y) / 64)))) {
		//removes the x movement from the tile and adds a movement in the y direction
		if (current.x < final.x) {
			nextTile.x = nextTile.x - 64;
			if (current.y < final.y) nextTile.y = current.y + 64;
			else if (current.y > final.y) nextTile.y = current.y - 64;
		}
		else if (current.x > final.x) {
			nextTile.x = nextTile.x + 64;
			if (current.y < final.y) nextTile.y = current.y + 64;
			else if (current.y > final.y) nextTile.y = current.y - 64;
		}
	}
	//moves x first
	if (current.x != nextTile.x) {
		if (nextTile.x > current.x) {
			current.x = current.x + movementSpeed; //adds movement to current in x
			g_cRenderWorld.drawAnimated(temp->frames[1], 200, 4, current); //draw RIGHT 1
		}
		else if (nextTile.x < current.x) {
			current.x = current.x - movementSpeed; //adds movement to current in x
			g_cRenderWorld.drawAnimated(temp->frames[2], 200, 4, current); //draw LEFT 2
		}
	}
	else if (current.y != nextTile.y) {
		if (nextTile.y > current.y) {
			current.y = current.y + movementSpeed; //adds movement to current in y
			g_cRenderWorld.drawAnimated(temp->frames[3], 200, 4, current); //draw DOWN 3
		}
		else if (nextTile.y < current.y) {
			current.y = current.y - movementSpeed; //adds movement to current in y
			g_cRenderWorld.drawAnimated(temp->frames[4], 200, 4, current); //draw UP 4
		}
	}
}

void graySwitch(int x){
	//unit* temp = new unit;
	//temp = startMap->moveSelected;

	if (x == 1) {
		//g_cRenderWorld.drawAnimated(playerArray[4]->frames[5], 200, 4, final); //gray idle
	}
	else if (x == 0){
		//g_cRenderWorld.drawAnimated(playerArray[4]->frames[0], 200, 4, final); //normal idle
	}
}

void MapBuilder(){

	int i;
	int j;
	Vector3 mapLoc;
	for (i = 0; i < 15; i++) {
		for (j = 0; j < 10; j++) {
			int xpos = 32 + (i * 64);
			int ypos = 640 - (32 + (j * 64));
			mapLoc.x = (float)xpos;
			mapLoc.y = (float)ypos;
			mapLoc.z = 1;
			g_cRenderWorld.draw(startMap->drawMap[i][j], mapLoc);
			if (startMap->unitMap[i][j] != nullptr) {
				unit* temp = startMap->unitMap[i][j];
				mapLoc.z = 0;
				if(temp->hasMoved() && !canAttack) g_cRenderWorld.drawAnimated(temp->frames[5], 250, 4, mapLoc);
				else  g_cRenderWorld.drawAnimated(temp->frames[0], 250, 4, mapLoc);
			}
		}
	}

	if (moveUnit) {
		unitMoveAlgorithm();
	}

	if (shAnimation) {
		ShootingAnimation();
	}

	if (exAnimation){
		ExplosionAnimation();
	}

	if (gameWinner) {
		WinnerAnimation();
	}

	if (turn == 1) {// desplays the back ground for player 1
		Vector3 sidebar;
		sidebar.x = 1056;
		sidebar.y = 320;
		sidebar.z = -1;

		g_cRenderWorld.draw(SIDE1, sidebar);

	}
	else if (turn == 2) {// desplays the back ground for player 2
		Vector3 sidebar;
		sidebar.x = 1056;
		sidebar.y = 320;
		sidebar.z = -1;

		g_cRenderWorld.draw(SIDE2, sidebar);
	}

}


void backgroundMusic(int song){
	if (song == TITLE){
		g_pSoundManager->loop(song);
	}
	else if (song == MATCH){
		g_pSoundManager->stop(TITLE);
		g_pSoundManager->loop(song);
	}

}

void RenderFrame(){
  g_cRenderWorld.BeginFrame();
  if (splashScreen) {
	  g_cRenderWorld.LoadSplash();
	  g_cRenderWorld.DrawBackground();
	  backgroundMusic(TITLE);
	  Vector3 mapLoc;
	  MapBuilder();
	  PrintButton();
  }
  else {
	  Move();
	  backgroundMusic(MATCH);
	  MapBuilder();
	  Button();
	  BCrystle();
	  //UnitManager(a, b);
	  SelectManager();
	  /* Old Hardcoding z
	  Vector3 solider;
	  solider.x = (float)(68.26 * 14 - (68.26/2));
	  solider.y = (float)(768 - 76.8 * 4 - (76.8/2));
	  solider.z = 0;
	  g_cRenderWorld.draw(BLUE, solider);
	  */
	  
  }
  g_cRenderWorld.EndFrame(); //shut down graphics pipeline
} //RenderFrame]

/// Process a frame of animation.
/// Called once a frame to animate game objects and take appropriate
/// action if the player has won or lost.

void ProcessFrame(){
  //stuff that gets done on every frame
  g_cTimer.beginframe(); //capture current time
  RenderFrame(); //render a frame of animation
} //ProcessFrame

void GridMaker(long x, long y){ //obsolete grid sectioning function
	/* Original Hardcoding here
	if (x > 0 && x < 68.26 && y > 0 && y < 76.8){
		MessageBox(0, "The cursor is in 1", "CellFound!", 0);

	}
	else if (x > 68.26 && x < 136.52 && y > 76.8 && y < 153.6){
		MessageBox(0, "The cursor is in 2", "CellFound!", 0);

	}
	else if (x > 136.52 && x < 204.78 && y > 153.6 && y < 230.4){
		MessageBox(0, "The cursor is in 3", "CellFound!", 0);

	}
	*/
	int xc = 0;
	int yc = 0;
	char txt[25];

	xc = (int)(x / 64) + 1;
	yc = (int)(y / 64) + 1;

	sprintf_s(txt, "(%d, %d)", xc, yc);

	MessageBox(0, txt, "CellFound!", 0);
}

void CellPrint(){//Zac: Captures cell location now using for debugging purposes

	char txt[100];
	//int pla;

	sprintf_s(txt, "Click: %d Player: %d C: %d D: %d play: %d \n\n A: %d B: %d \n\n canCapture: %d", cLevel, startMap->unitPlayerGrabber(a-1,b-1), c, d, play, a-1, b-1);
	MessageBox(0, txt, "Leveler", 0);
	
	//for (int i = 0; i < 5; i++)
	//{
	//ter = soldierArray[i]->printTerrain();
	//sprintf_s(txt, "Unit %d: %d", i, ter);
	//MessageBox(0, txt, "Unit Terrain!", 0);
	//}

}

void baseCapture(){
	char pos[100];

	if (!startMap->isUnit(0, 4) || !startMap->isUnit(14, 4)){
		canCapture = false;
		baseHealth = 200;
	}
	if ((a - 1) == 0 && (b - 1) == 4 && startMap->unitPlayerGrabber(c, d) == 2){
		canCapture = true;
		targetBase = 1;
		//sprintf_s(pos, "red base reached");
		//MessageBox(0, pos, "Enemy base", 0);
	}
	if ((a - 1) == 14 && (b - 1) == 4 && startMap->unitPlayerGrabber(c, d) == 1){
		canCapture = true;
		targetBase = 2;
		//sprintf_s(pos, "blue base reached");
		//MessageBox(0, pos, "Enemy base", 0);
	}
	startMap->checkHome(a - 1, b - 1, c, d);
}


//this function allows for different levels of priorities for clicking as the game runs z
int clickLeveler(int click, int x, int y){
	char pos[50];
	int fuckchecker = 0;
	
	switch (click)
	{
	case 0: //click level 0: only used for splash screen z
		if (splashScreen) {
			if (x >= (960 / 2) - 150 && x <= (960 / 2) + 150 && y >= 640 - 250 && y <= 640 - 150) {
				splashScreen = false;
				ShowCursor(FALSE);
				click = 1;
			}
		}; 
		return click;
		break;

	case 1: //click level 1: all ows for unit selection and ending player turn z


		if (startMap->isUnit(a - 1, b - 1) && !startMap->isSelected() && !startMap->unitMap[a-1][b-1]->hasMoved() && startMap->unitMap[a-1][b-1]->returnPlayer() == turn) { //when clicked checks spot for unit
			//if (startMap->unitMap[a - 1][b - 1]->returnPlayer() != turn)
			//{
				//return click;
			//}
			startMap->selectUnit(a - 1, b - 1);//if spot is unit it selects it z
			c = a - 1;
			d = b - 1;
			play = startMap->unitMap[c][d]->returnPlayer(); //grabs selected units player ownership z
			click = 2;
		}
		else if (x > 960 && y > 576) { // turn switcher
			for (int i = 0; i < 6; i++)
			{
				if (playerArray[i]->isHome)
				{
					playerArray[i]->healing();
				}
				if (playerArray[i]->hasMoved())
				{
					playerArray[i]->changeMoved();
				}
			}
			if (turn == 2) 
				turn = 1;
			else if (turn == 1)
				turn = 2;
			canAttack = false;
		};
		return click;
		break;

	case 2: //click level 2: allows for unit movement after selection, unit waiting, and unit attack targeting buttons z
		if (x < 960 && !moveUnit && !exAnimation && !shAnimation){
			if (startMap->isUnit(a - 1, b - 1) && startMap->unitPlayerGrabber(a - 1, b - 1) != turn)
			{
				click = 1;
				startMap->deselectUnit();
				return click;
			}
			if (startMap->canMove(a - 1, b - 1)) { //checks if unit can be moved;
				startMap->unitMap[c][d]->changeMoved();
				baseCapture();
				moveUnit = true;
				unit* temp = startMap->unitMap[startMap->xselected][startMap->yselected];
				if (temp->printType() == 3) g_pSoundManager->loop(TANK);
				else g_pSoundManager->loop(STEPS);
				startMap->unitMap[startMap->xselected][startMap->yselected] = nullptr;
				current.x = (float)((startMap->xselected * 64) + 32);
				current.y = (float)((640 - 32) - (startMap->yselected * 64));
				current.z = -1;
				nextTile = current;
				final.x = (float)(((a - 1) * 64) + 32);
				final.y = (float)((640 - 32) - ((b - 1) * 64));
				final.z = -1;
				//if the unit is diagonal (helps with how the redirection in the algorithm works)
				if (startMap->isUnit(startMap->xselected - 1, startMap->yselected - 1) || startMap->isUnit(startMap->xselected - 1, startMap->yselected + 1) ||
					startMap->isUnit(startMap->xselected + 1, startMap->yselected - 1) || startMap->isUnit(startMap->xselected + 1, startMap->yselected + 1)){
					if (current.x < final.x) nextTile.x = current.x + 64;
					else if (current.x > final.x) nextTile.x = current.x - 64;
					if (current.y < final.y) nextTile.y = current.y + 64;
					else if (current.y > final.y) nextTile.y = current.y - 64;

					if (nextTile.y == final.y) {
						if (current.y < final.y) nextTile.y -= 64;
						else if (current.y > final.y) nextTile.y += 64;
					}
				}
				//sets the next tile to move if no diagonal tile
				else {
					if (current.x < final.x) nextTile.x = current.x + 64;
					else if (current.x > final.x) nextTile.x = current.x - 64;
					else if (current.y < final.y) nextTile.y = current.y + 64;
					else if (current.y > final.y) nextTile.y = current.y - 64;
				}
				
				c = a - 1;
				d = b - 1;
				
				if (startMap->checkTarget(c, d, play))
				{
					//c = a - 1;
					//d = b - 1;
					//sprintf_s(pos, "found target!!!");
					//MessageBox(0, pos, "oh shit", 0);
					canAttack = true;
					click = 3;
					return click;
				}
				if (canCapture){
					click = 3;
					//sprintf_s(pos, "ready to capture");
					//MessageBox(0, pos, "Enemy base", 0);
					return click;
				}
				else
				{
					canAttack = false;
					click = 1;
				}
			}
			
			if (a-1 == c && b-1 == d)
			{
				if (startMap->checkTarget(c, d, play))
				{
					canAttack = true;
					click = 3;
					return click;
				}
				if (canCapture){
					click = 3;
					return click;
				}
				else
				{
					click = 1;
					//return click;
				}
			}
			else
			{
				click = 1;
				//return click;
			}
		}
		else if (x >= 960 && y >= 108 && y <= 169){ // ATTACK BUTTON IS CLICKED
			if (startMap->checkTarget(c, d, play))
			{
				click = 3;
				//target highlighting happens here
			}
		}
		else if (x >= 960 && y >= 208 && y <= 269){
			//code for wait buttin
			//graySwitch(1);
			startMap->unitMap[c][d]->changeMoved();
			startMap->deselectUnit();
			click = 1;
		}
		else if (x >= 960 && y >= 308 && y <= 369){// capture button is clicked
			if (canCapture){
				click = 1;
				//sprintf_s(pos, "Base Health: %d", baseHealth);
				//MessageBox(0, pos, "Enemy base", 0);
				//winner = play;
				//sprintf_s(pos, "Player %d", play);
				//MessageBox(0, pos, "WINNER", 0);
			}
		}

		return click;
		break;

	case 3: //click level 3 allows for use of turn switcher, and attacking of a clicked target z

		if (x > 960 && y > 576) { // turn switcher
			startMap->deselectUnit();
			for (int i = 0; i < 6; i++)
			{
				if (playerArray[i]->hasMoved())
				{
					playerArray[i]->changeMoved();
				}
			}
			if (turn == 2)
				turn = 1;
			else if (turn == 1)
				turn = 2;

			click = 1;
			//return click;
		}
		else if (x >= 960 && y >= 108 && y <= 169){ // ATTACK BUTTON IS CLICKED
			if (startMap->checkTarget(c, d, play))
			{
				click = 4;
				showCrosshair = true;
			}
		}
		else if (x >= 960 && y >= 208 && y <= 269){
			//code for wait buttin
			graySwitch(1);
			startMap->unitMap[c][d]->changeMoved();
			startMap->deselectUnit();
			click = 1;
		}
		else if (x >= 960 && y >= 308 && y <= 369){ //capture button is clicked
			if (canCapture){
				baseHealth = baseHealth - 200;
				click = 1;
				//sprintf_s(pos, "Base Health: %d", baseHealth);
				//MessageBox(0, pos, "Enemy base", 0);
				if (baseHealth == 0)
				{
					winner = play;
					gameWinner = true;
					g_pSoundManager->play(EXPLOSION);
					ShowCursor(TRUE);
					splashScreen = true;
					moveUnit = false;
					playerUnits[0] = 0;
					playerUnits[1] = 0;
					sprintf_s(pos, "Player %d", winner);
					click = 0;
				}
			}
		}

		return click;
		break;

	case 4: 
		if (startMap->isUnit(a - 1, b - 1) && startMap->unitPlayerGrabber(a - 1, b - 1) != play){
			bool death;
			int tar = startMap->unitPlayerGrabber(a - 1, b - 1);
			death = startMap->attackUnit(a - 1, b - 1, c, d); //a,b = target; c,d = attacker
			explosion.x = (a - 1) * 64 + 32;
			explosion.y = (640 - (64 * (b) - 32));
			explosion.z = -7;

			if (!death) {
				shAnimation = true;
				g_pSoundManager->play(SHOTS);
				canAttack = false;
				showCrosshair = false;
			}
			
			if (death) {
				exAnimation = true;
				g_pSoundManager->play(EXPLOSION);
				canAttack = false;
				showCrosshair = false;
			}
			if (death){
				if (!startMap->isUnit(a - 1, b - 1)){
					playerUnits[tar - 1]--;
				}
				else{
					playerUnits[play - 1]--;
				}
			}
			if (playerUnits[tar - 1] == 0){
				winner = play;
				gameWinner = true;
				g_pSoundManager->play(EXPLOSION);
				ShowCursor(TRUE);
				splashScreen = true;
				moveUnit = false;
				playerUnits[0] = 0;
				playerUnits[1] = 0;
				sprintf_s(pos, "Player %d", winner);
				MessageBox(0, pos, "WINNER", 0);
				click = 0;
			}
			else if (playerUnits[play - 1] == 0){
				winner = tar;
				gameWinner = true;
				g_pSoundManager->play(EXPLOSION);
				ShowCursor(TRUE);
				splashScreen = true;
				moveUnit = false;
				playerUnits[0] = 0;
				playerUnits[1] = 0;
				sprintf_s(pos, "Player %d", winner);
				MessageBox(0, pos, "WINNER", 0);
				click = 0;
			}
			else{
				click = 1;
			}
		}
		else
			click = 1;
		return click;
		break;

	default:
		break;
	}
}

void MouseFunc(){ //helper function for the mouse handler
	long x = 0;
	long y = 0;


	GetCursorPos(&poi);
	ScreenToClient(g_HwndApp, &poi);
	//char pos[50];
	x = poi.x;
	y = poi.y;

	cLevel = clickLeveler(cLevel, x, y);
}

 //Zac: THE MOUSE HANDLER WORRRKS!
BOOL MouseHandler(WPARAM m){ //mouse handler
	switch (m){
	case VK_LBUTTON: MouseFunc(); break;
	} 
	return FALSE;
} //mouse handler


/// Keyboard handler.
/// Take the appropriate action when the user mashes a key on the keyboard.
///  \param k Virtual key code for the key pressed
///  \return TRUE if the game is to exit



BOOL KeyboardHandler(WPARAM k){ //keyboard handler
	switch(k){
    case VK_ESCAPE: return TRUE; //quit       
    case VK_RETURN: BeginGame(); break; //start again
	case VK_SPACE: MapBuilder(); break; //bamdamn wayme
	case VK_SHIFT: break; //show units complete info startMap->getUnitInfo(a-1, b-1)
	case VK_CONTROL: break; break; //startMap->attackUnit(a-1, b-1); break; //print loc of func in window!  CellPrint()
	case VK_BACK: break;
	//case WM_LBUTTONDOWN: PrintWin(); break; //not working for left mouse click
  } //switch
  return FALSE;
} //KeyboardHandler



// Windows functions.
// Dont mess with these unless you really know what you're doing.
// I've written default functions in the Engine library to take
// care of the boring details of Windows housekeeping.

/// Window procedure.
/// Handler for messages from the Windows API. Dont mess with these unless you really know what you're doing.
///  \param h window handle
///  \param m message code
///  \param w parameter for message 
///  \param l second parameter for message
///  \return 0 if message is handled

LRESULT CALLBACK WindowProc(HWND h, UINT m, WPARAM w, LPARAM l){
  return DefaultWindowProc(h, m, w, l);
} //WindowProc

/// Winmain.  
/// Main entry point for this application. Dont mess with these unless you really know what you're doing.
///  \param hI handle to the current instance of this application
///  \param hP unused
///  \param lpC unused 
///  \param nCS specifies how the window is to be shown
///  \return TRUE if application terminates correctly

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nCS){    
  ShowCursor(FALSE);
  return DefaultWinMain(hI, hP, lpC, nCS);
} //WinMain

//Team 0 uses theres art assets for educational puposes only and does not own all artwork used in the game.