#ifndef SPEEDRUN_H
#define SPEEDRUN_H

extern uint16_t nextDir[300];
extern int32_t length[300];

int32_t speedRunClassic(uint16_t targetX, uint16_t targetY);
int32_t speedRunDiag(uint16_t targetX, uint16_t targetY);
int32_t speedRunSmooth(uint16_t targetX, uint16_t targetY);
int32_t generatePathNESW(uint16_t targetX, uint16_t targetY);
int32_t generatePathSLR(uint16_t targetX, uint16_t targetY);
int32_t generatePathDiag(uint16_t targetX, uint16_t targetY);
int32_t generatePathSmooth(uint16_t targetX, uint16_t targetY);
uint16_t getNextDirection(uint16_t xPos, uint16_t yPos);
void closeUntracedCells(void);
int32_t countDiagonals(uint32_t startDir, uint32_t head);

#define SS0 51
#define SS90L 52
#define SS90R 53
#define SS180L 54
#define SS180R 55
#define SD45L 56
#define SD45R 57
#define SD135L 58
#define SD135R 59
#define DS45L 60
#define DS45R 61
#define DS135L 62
#define DS135R 63
#define DD90L 64
#define DD90R 65
#define DD0 66

#endif

