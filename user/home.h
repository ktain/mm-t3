#ifndef HOME_H
#define HOME_H

extern bool busy;
extern int32_t signalThreshold;
extern int32_t leftCount;
extern int32_t rightCount;

bool waitForSignal(void);
void refreshOptions(void);
void enter(void);
void back(void);

extern char* options[];

extern int32_t startX;
extern int32_t startY;
extern int32_t destX1;
extern int32_t destY1;
extern int32_t destX2;
extern int32_t destY2;
extern int32_t destX3;
extern int32_t destY3;
extern int32_t destX4;
extern int32_t destY4;

#endif
