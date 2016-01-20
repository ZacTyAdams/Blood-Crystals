#include "Base.h"
#include "gamedefines.h"

base::base() {
	//This declare diffent types of maptiles to use later.
	
	//Grasses
	grass0.declare(GRASS0, GRASS0_R, GRASS0_B, GRASS0); //regular tile, red tile, blue tile, terrain type
	grass1.declare(GRASS1, GRASS1_R, GRASS1_B, GRASS0);
	
	//Trees
	tree0.declare(TREE0, TREE0_R, TREE0_B, TREE0);
	tree1.declare(TREE1, TREE1_R, TREE1_B, TREE0);

	//Mountains
	mountain0.declare(MOUNTAIN0, MOUNTAIN0_R, MOUNTAIN0_B, MOUNTAIN0);
	mountain1.declare(MOUNTAIN1, MOUNTAIN1_R, MOUNTAIN1_B, MOUNTAIN0);
	mountain2.declare(MOUNTAIN2, MOUNTAIN2_R, MOUNTAIN2_B, MOUNTAIN0);
	mountain3.declare(MOUNTAIN3, MOUNTAIN3_R, MOUNTAIN3_B, MOUNTAIN0);
	mountain4.declare(MOUNTAIN4, MOUNTAIN4_R, MOUNTAIN4_B, MOUNTAIN0);
	mountain5.declare(MOUNTAIN5, MOUNTAIN5_R, MOUNTAIN5_B, MOUNTAIN0);

	//Bases
	redBase.declare(RED_BASE, RED_BASE_R, RED_BASE_B, RED_BASE);
	blueBase.declare(BLUE_BASE, BLUE_BASE_R, BLUE_BASE_B, BLUE_BASE);

	//Roads
	road0.declare(ROAD0, ROAD0_R, ROAD0_B, ROAD0);
	road1.declare(ROAD1, ROAD1_R, ROAD1_B, ROAD0);
	road2.declare(ROAD2, ROAD2_R, ROAD2_B, ROAD0);
	road3.declare(ROAD3, ROAD3_R, ROAD3_B, ROAD0);
	road4.declare(ROAD4, ROAD4_R, ROAD4_B, ROAD0);
	road5.declare(ROAD5, ROAD5_R, ROAD5_B, ROAD0);
	road6.declare(ROAD6, ROAD6_R, ROAD6_B, ROAD0);
	road7.declare(ROAD7, ROAD7_R, ROAD7_B, ROAD0);
	road8.declare(ROAD8, ROAD8_R, ROAD8_B, ROAD0);
	road9.declare(ROAD9, ROAD9_R, ROAD9_B, ROAD0);
	road10.declare(ROAD10, ROAD10_R, ROAD10_B, ROAD0);

}