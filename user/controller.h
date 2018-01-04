#ifndef CONTROLLER_H
#define CONTROLLER_H

void curveLeft90(void);
void curveLeft90SS(void);
void curveLeft90DD(void);
void curveRight90(void);
void curveRight90SS(void);
void curveRight90DD(void);
void curveLeft45SD(void);
void curveLeft45DS(void);
void curveRight45SD(void);
void curveRight45DS(void);
void curveLeft135SD(void);
void curveLeft135DS(void);
void curveRight135SD(void);
void curveRight135DS(void);
void curveLeft180SS(void);
void curveRight180SS(void);
void pivotLeft90(void);
void pivotRight90(void);
void pivotLeft180(void);
void pivotRight180(void);
void moveN(void);
void moveE(void);
void moveS(void);
void moveW(void);
void moveBack(void);
void alignFrontWall(void);
void pivotN(void);
void pivotE(void);
void pivotS(void);
void pivotW(void);
bool approachingWall(void);
float getStartSpeed(int32_t profile);
float getEndSpeed(int32_t profile);

#endif

