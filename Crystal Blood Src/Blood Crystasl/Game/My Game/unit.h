//unit class
class unit {
private:
	friend class map; //allows map to edit
	int moveDistance; //stores how many spaces it is able to move
	int health; //integer for the unit health z
	int unitType; //integer for unit type z
	int terrain; //integer for terrain type z
	int home; //units location in map 0=x 1=y z
	

public:
	bool idle;
	int frames[6][4];
	//Idle        frames[0]
	//Left Move   frames[1]
	//Right Move  frames[2]
	//Up Move     frames[3]
	//Down Move   frames[4]
	//Moved (grey)frames[5]
	unit(); //constructor
	int returnMoveDistance(); //returns the move distance
	int returnPlayer(); //returns which player (1, or 2)
	void makeSoldier(int p); //makes unit into a soldier
	void makeHeavySoldier(int p); //makes unit into a heavy soldier
	void makeTank(int p); //makes unit into a tank
	void getTerrain(int t); //sets the current terrain the unit is at
	void getLocation(int x, int y); //gets the current location of the unit
	int printTerrain();
	int printHealth();
	int printType();
	void takeDamage(int dam);
	void changeMoved();
	bool hasMoved();
	bool returnCap();
	bool setHome();
	void healing();


	bool Moved; //Tracks weather the unit has moved or not
	int player;
	bool isHome;
	bool canCap;
};