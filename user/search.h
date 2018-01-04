#ifndef SEARCH_H
#define SEARCH_H

int32_t floodSearch(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4);
void performNextMove(void);
void simulateStraight(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4);
int32_t floodSearchPivot(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4);
void performNextMovePivot(void);
void updateDistances(void);
void updateDistancesComplete(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4);
void detectWalls(void);

struct cellStack popCell(void);
void pushCell(uint16_t x, uint16_t y);
void pushAdjacentCell(uint16_t x, uint16_t y, uint16_t wall);

struct cellStack popCellVisit(void);
void pushCellVisit(uint16_t x, uint16_t y);
bool hasPathToTarget(uint16_t xPos, uint16_t yPos);

extern bool useBacktracking;
extern bool updateAdjacentCells;
extern int32_t pivotTurnDelay;
extern int16_t destinationX;
extern int16_t destinationY;
extern int32_t detectWallOvershoot;

#endif
