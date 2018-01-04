#ifndef MODEL_H
#define MODEL_H

// Bit identifiers
#define NORTH 1
#define EAST 2
#define SOUTH 4
#define WEST 8
#define TRACEBIT 16

// Other identifiers
#define STRAIGHT 101
#define LEFT 102
#define RIGHT 103

extern uint16_t cell[SIZEX][SIZEY];	// cell[x][y]
extern int32_t distance[SIZEX][SIZEY];	// distance of cell[x][y]
extern int32_t curPosX;		// 0 bottom left, horizontal
extern int32_t curPosY;		// 0 bottom left, vertical
extern uint16_t orientation;

void initializeMaze(void);
void placeWall(uint16_t x, uint16_t y, uint16_t direction);
void placeTrace(uint16_t x, uint16_t y);
	
bool hasTrace(uint16_t x, uint16_t y);
bool hasNorthWall(uint16_t x, uint16_t y);
bool hasEastWall(uint16_t x, uint16_t y);
bool hasSouthWall(uint16_t x, uint16_t y);
bool hasWestWall(uint16_t x, uint16_t y);

bool atTarget(uint16_t targetX, uint16_t targetY);
int32_t getMinDistance(uint16_t xPos, uint16_t yPos);
int32_t getMinDir(uint16_t xPos, uint16_t yPos);
#endif

