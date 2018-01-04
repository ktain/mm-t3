#include "main.h"

uint16_t cell[SIZEX][SIZEY];	// cell[x][y]
int32_t distance[SIZEX][SIZEY];	// distance of cell[x][y]
int32_t curPosX;		// 0 bottom left, horizontal
int32_t curPosY;		// 0 bottom left, vertical
uint16_t orientation;

// Place known wall information
void initializeMaze(void) {

	placeWall(0, 0, EAST);	// east wall at starting cell

	// Place outer north and south walls
	for (int32_t i = 0; i < SIZEX; i++) {
		placeWall(i, SIZEY-1, NORTH);	// north wall
		placeWall(i, 0, SOUTH);		// south wall
	}

	// Place outer east and west walls
	for (int32_t i = 0; i < SIZEY; i++) {
		placeWall(SIZEX-1, i, EAST);	// east wall
		placeWall(0, i, WEST);		// west wall
	}

}

// Place a wall, direction can be bitwise or'ed
void placeWall(uint16_t x, uint16_t y, uint16_t direction) {
	cell[x][y] |= direction;
	if ( (x > 0) && (direction == WEST) )
		cell[x-1][y] |= EAST;
	if ( (x < SIZEX-1) && (direction == EAST) )
		cell[x+1][y] |= WEST;
	if ( (y > 0) && (direction == SOUTH) )
		cell[x][y-1] |= NORTH;
	if ( (y < SIZEY - 1) && (direction == NORTH) )
		cell[x][y+1] |= SOUTH;
}

// Place a trace
void placeTrace(uint16_t x, uint16_t y) {
	cell[x][y] |= TRACEBIT;
}

// Check for trace
bool hasTrace(uint16_t x, uint16_t y) {
	return (cell[x][y] & TRACEBIT);
}

// Check for north wall
bool hasNorthWall(uint16_t x, uint16_t y) {
	return (cell[x][y] & NORTH);
}

// Check for east wall
bool hasEastWall(uint16_t x, uint16_t y) {
	return (cell[x][y] & EAST);
}

// Check for south wall
bool hasSouthWall(uint16_t x, uint16_t y) {
	return (cell[x][y] & SOUTH);
}

// Check for west wall
bool hasWestWall(uint16_t x, uint16_t y) {
	return (cell[x][y] & WEST);
}

// Check if current position is at target
bool atTarget(uint16_t targetX, uint16_t targetY) {
	return (curPosX == targetX && curPosY == targetY);
}

// Get smallest distance of surrounding cells
int32_t getMinDistance(uint16_t xPos, uint16_t yPos) {
	int32_t min;
	int32_t distN;
	int32_t distE;
	int32_t distS;
	int32_t distW;

	distN = hasNorthWall(xPos, yPos)? MAX_DIST : distance[xPos][yPos+1];
	distE = hasEastWall(xPos, yPos)? MAX_DIST : distance[xPos+1][yPos];
	distS = hasSouthWall(xPos, yPos)? MAX_DIST : distance[xPos][yPos-1];
	distW = hasWestWall(xPos, yPos)? MAX_DIST : distance[xPos-1][yPos];

	min = distN;
	min = (distE < min)? distE : min;
	min = (distS < min)? distS : min;
	min = (distW < min)? distW : min;

	return min;
}

int32_t getMinDir(uint16_t xPos, uint16_t yPos) {
	int32_t minDir;
	int32_t distN;
	int32_t distE;
	int32_t distS;
	int32_t distW;

	distN = hasNorthWall(xPos, yPos)? MAX_DIST : distance[xPos][yPos+1];
	distE = hasEastWall(xPos, yPos)? MAX_DIST : distance[xPos+1][yPos];
	distS = hasSouthWall(xPos, yPos)? MAX_DIST : distance[xPos][yPos-1];
	distW = hasWestWall(xPos, yPos)? MAX_DIST : distance[xPos-1][yPos];

	minDir = NORTH;
	minDir = (distE < distN)? EAST : NORTH;
	minDir = (distS < distE)? SOUTH : EAST;
	minDir = (distW < distS)? WEST : SOUTH;
	
	return minDir;
}

