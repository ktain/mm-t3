#include "main.h"

/**
 * Pivot turn profiles
 * speedW angle delay
 */
struct pivotTurnProfile pivotLeft90Profile[3] = {
	-500, -71800, 100,	// 0	// -73250 7.6v
	-500, -71800, 70,	// 1
	-500, -71800, 40,	// 2
};

struct pivotTurnProfile pivotRight90Profile[3] = {
	500, 71800, 100,	// 0	// 73050 7.6v
	500, 71800, 70,		// 1
	500, 71800, 40,		// 2
};

struct pivotTurnProfile pivotLeft180Profile[3] = {
	-500, -147700, 100,	// 0	// -149300 7.6v
	-500, -147700, 70,	// 1
	-500, -147700, 40	// 2
};

struct pivotTurnProfile pivotRight180Profile[3] = {
	500, 147700, 100,	// 0	// 149100 7.6v
	500, 147700, 70,	// 1
	500, 147700, 40,	// 2
};

/**
 * Curve turn profiles
 * d1 t1 t2 t3 d2 startSpeed speedX endSpeed speedW accW decW kpW13 kdW13 kpW2 kdW2
 */
 
// Search turns
struct turnProfile curveLeft90Profile[3] = {
	20, 30, 168, 30, 20, 0.5, 0.5, 0.5, -456, 15000, 15000, 1.0, 15, 1.0, 15,		// 0
	20, 52, 63, 52, 20, 0.7, 0.7, 0.7, -782, 15000, 15000, 1.0, 20, 1.0, 20,		// 1
	15, 39, 76, 39, 15, 0.8, 0.8, 0.8, -780, 20000, 20000, 1.0, 20, 1.0, 20,		// 2
};

struct turnProfile curveRight90Profile[3] = {
	20, 30, 168, 30, 20, 0.5, 0.5, 0.5, 456, 15000, 15000, 1.0, 15, 1.0, 15,		// 0
	20, 52, 63, 52, 20, 0.7, 0.7, 0.7, 782, 15000, 15000, 1.0, 20, 1.0, 20,			// 1
	15, 39, 76, 39, 15, 0.8, 0.8, 0.8, 780, 20000, 20000, 1.0, 20, 1.0, 20,			// 2
};

/**
 * Diagonal run turn profiles
 * d1 t1 t2 t3 d2 startSpeed speedX endSpeed speedW accW decW kpW13 kdW13 kpW2 kdW2
 */

// Straight to straights
struct turnProfile curveLeft90SSProfile[3] = {
	18, 41, 179, 41, 50, 1.0, 1.0, 1.0, -410, 10000, 10000, 0.7, 20, 0.7, 25,	// 0
	18, 63, 80, 63, 50, 1.3, 1.3, 1.3, -630, 10000, 10000, 1.2, 15, 1.2, 15,
	18, 63, 80, 63, 50, 1.3, 1.3, 1.3, -630, 10000, 10000, 1.2, 15, 1.2, 15,
};

struct turnProfile curveRight90SSProfile[3] = {
	18, 41, 179, 41, 50, 1.0, 1.0, 1.0, 410, 15000, 15000, 0.7, 20, 0.7, 25,	// 0
	18, 63, 80, 63, 50, 1.3, 1.3, 1.3, 630, 10000, 10000, 1.2, 15, 1.2, 15,
	18, 63, 80, 63, 50, 1.3, 1.3, 1.3, 630, 10000, 10000, 1.2, 15, 1.2, 15,
};

struct turnProfile curveLeft180Profile[3] = {
	90, 43, 238, 43, 90, 1.0, 1.0, 1.0, -640, 15000, 15000, 0.7, 20, 0.7, 25,
	44, 42, 248, 52, 44, 1.1, 1.0, 1.1, -620, 15000, 15000, 0.7, 20, 0.7, 25,
	44, 42, 248, 52, 44, 1.1, 1.0, 1.1, -620, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight180Profile[3] = {
	90, 43, 238, 43, 90, 1.0, 1.0, 1.0, 640, 15000, 15000, 0.7, 20, 0.7, 25,
	44, 42, 251, 52, 44, 1.1, 1.0, 1.1, 620, 15000, 15000, 0.7, 20, 0.7, 25,
	44, 42, 251, 52, 44, 1.1, 1.0, 1.1, 620, 15000, 15000, 0.7, 20, 0.7, 25,
};

// Diagonals
struct turnProfile curveLeft90DDProfile[3] = {
	23, 52, 63, 52, 23, 0.7, 0.7, 0.7, -782, 15000, 15000, 0.8, 10, 0.8, 10,	// 0
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, -410, 5000, 5000, 0.7, 20, 0.7, 25,
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, -410, 5000, 5000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight90DDProfile[3] = {
	23, 52, 63, 52, 23, 0.7, 0.7, 0.7, 782, 15000, 15000, 0.8, 10, 0.8, 10,	// 0
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, 410, 5000, 5000, 0.7, 20, 0.7, 25,
	0, 82, 138, 82, 0, 0.7, 0.7, 0.7, 410, 5000, 5000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveLeft45SDProfile[3] = {
	0, 32, 62, 147, 0, 0.7, 0.7, 0.7, -480, 15000, 15000, 0.7, 20, 0.7, 25,	// 0
	2, 75, 75, 75, 33, 0.7, 0.7, 0.7, -300, 4000, 4000, 0.7, 20, 0.7, 25,
	2, 75, 75, 75, 33, 0.7, 0.7, 0.7, -300, 4000, 4000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight45SDProfile[3] = {
	0, 32, 62, 147, 0, 0.7, 0.7, 0.7, 480, 15000, 15000, 0.7, 20, 0.7, 25,	// 0
	10, 75, 75, 75, 27, 0.7, 0.7, 0.7, 300, 4000, 4000, 0.7, 20, 0.7, 25,
	10, 75, 75, 75, 27, 0.7, 0.7, 0.7, 300, 4000, 4000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveLeft45DSProfile[3] = {
	105, 30, 45, 30, 40, 0.7, 0.7, 0.7, -590, 20000, 20000, 0.7, 20, 0.7, 25,
	105, 30, 45, 30, 40, 0.7, 0.7, 0.7, -590, 20000, 20000, 0.7, 20, 0.7, 25,
	105, 30, 45, 30, 40, 0.7, 0.7, 0.7, -590, 20000, 20000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight45DSProfile[3] = {
	110, 30, 45, 30, 35, 0.7, 0.7, 0.7, 590, 20000, 20000, 0.7, 20, 0.7, 25,
	110, 30, 45, 30, 35, 0.7, 0.7, 0.7, 590, 20000, 20000, 0.7, 20, 0.7, 25,
	110, 30, 45, 30, 35, 0.7, 0.7, 0.7, 590, 20000, 20000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveLeft135SDProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight135SDProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
};


struct turnProfile curveLeft135DSProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, -580, 15000, 15000, 0.7, 20, 0.7, 25,
};

struct turnProfile curveRight135DSProfile[3] = {
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
	90, 39, 194, 160, 0, 0.7, 0.7, 0.7, 580, 15000, 15000, 0.7, 20, 0.7, 25,
};

void curveLeft90(void) {
	turn(curveLeft90Profile[searchProfile]);
}

void curveRight90(void) {
	turn(curveRight90Profile[searchProfile]);
}

void curveLeft90SS(void) {
	turnDiag(curveLeft90SSProfile[speedRunProfile]);
}

void curveRight90SS(void) {
	turnDiag(curveRight90SSProfile[speedRunProfile]);
}

void curveLeft90DD(void) {
	turnDiag(curveLeft90DDProfile[speedRunProfile]);
}

void curveRight90DD(void) {
	turnDiag(curveRight90DDProfile[speedRunProfile]);
}

void curveLeft45SD(void) {
	turnDiag(curveLeft45SDProfile[speedRunProfile]);
}

void curveRight45SD(void) {
	turnDiag(curveRight45SDProfile[speedRunProfile]);
}

void curveLeft45DS(void) {
	turnDiag(curveLeft45DSProfile[speedRunProfile]);
}

void curveRight45DS(void) {
	turnDiag(curveRight45DSProfile[speedRunProfile]);
}

void curveLeft135SD(void) {
	turnDiag(curveLeft135SDProfile[speedRunProfile]);
}

void curveRight135SD(void) {
	turnDiag(curveRight135SDProfile[speedRunProfile]);
}

void curveLeft135DS(void) {
	turnDiag(curveLeft135DSProfile[speedRunProfile]);
}

void curveRight135DS(void) {
	turnDiag(curveRight135DSProfile[speedRunProfile]);
}

void curveLeft180SS(void) {
	turnDiag(curveLeft180Profile[speedRunProfile]);
}

void curveRight180SS(void) {
	turnDiag(curveRight180Profile[speedRunProfile]);
}

void pivotLeft90(void) {
	pivotTurn(pivotLeft90Profile[searchProfile]);
}

void pivotRight90(void) {
	pivotTurn(pivotRight90Profile[searchProfile]);
}

void pivotLeft180(void) {
	pivotTurn(pivotLeft180Profile[searchProfile]);
}

void pivotRight180(void) {
	pivotTurn(pivotRight180Profile[searchProfile]);
}

void moveN(void) {
	if (orientation == NORTH)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == EAST)
		curveLeft90();
	else if (orientation == SOUTH)
		moveBack();
	else if (orientation == WEST)
		curveRight90();
	
	curPosY++;
	orientation = NORTH;
}

void moveE(void) {
	if (orientation == EAST)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == SOUTH)
		curveLeft90();
	else if (orientation == WEST)
		moveBack();
	else if (orientation == NORTH)
		curveRight90();
	
	curPosX++;
	orientation = EAST;
}

void moveS(void) {
	if (orientation == SOUTH)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == WEST)
		curveLeft90();
	else if (orientation == NORTH)
		moveBack();
	else if (orientation == EAST)
		curveRight90();
	
	curPosY--;
	orientation = SOUTH;
}

void moveW(void) {
	if (orientation == WEST)
		moveStraight(1, searchSpeed, searchSpeed);
	else if (orientation == NORTH)
		curveLeft90();
	else if (orientation == EAST)
		moveBack();
	else if (orientation == SOUTH)
		curveRight90();
	
	curPosX--;
	orientation = WEST;
}

void moveBack(void) {

	bool hasFrontWall = (LFSensorT > frontWallThresholdL || RFSensorT > frontWallThresholdR)? 1: 0;
	bool hasLeftWall = (LDSensor > leftWallThreshold)? 1: 0;
	bool hasRightWall = (RDSensor > rightWallThreshold)? 1: 0;

	int32_t LDOffset = LDMidVal - LDSensor;	// Higher = further away
	int32_t RDOffset = RDMidVal - RDSensor;
	
	// Decelerate half a cell distance
	if (hasFrontWall && useAlignment) {
		moveStraight(0.5, turnSpeed, stopSpeed);
		delay_ms(alignTime);
		align();
	}
	else {
		moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, turnSpeed, stopSpeed);
		delay_ms(50);
	}
	
	// Turn back
	if (hasLeftWall && (LDOffset > RDOffset) && useAlignment) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	else if (hasRightWall && (RDOffset > LDOffset) && useAlignment) {
		pivotRight90();
		align();
		pivotRight90();
	}
	else if (hasLeftWall && useAlignment) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	else if (hasRightWall && useAlignment) {
		pivotRight90();
		align();
		pivotRight90();
	}
	else {
		// Randomly pivot 180
		(millis()%2)? pivotLeft180():pivotRight180();
	}
	
	// Move forward half a cell distance
	moveStraight(0.5, turnSpeed, turnSpeed);
}


// Pivot the mouse north
void pivotN(void) {
	if (orientation == EAST)
		pivotLeft90();
	else if (orientation == SOUTH) {
		// If has left wall, align with left wall
		if (useAlignment && hasEastWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasWestWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == WEST)
		pivotRight90();
	
	orientation = NORTH;
}


// Pivot the mouse east
void pivotE(void) {
	if (orientation == NORTH)
		pivotRight90();
	else if (orientation == SOUTH)
		pivotLeft90();
	else if (orientation == WEST) {
		// If has left wall, align with left wall
		if (useAlignment && hasSouthWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasNorthWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}

	orientation = EAST;
}


// Pivot the mouse south
void pivotS(void) {
	if (orientation == NORTH) {
		// If has left wall, align with left wall
		if (useAlignment && hasWestWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasEastWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == EAST)
		pivotRight90();
	else if (orientation == WEST)
		pivotLeft90();

	orientation = SOUTH;
}


// Pivot the mouse west
void pivotW(void) {
	if (orientation == NORTH)
		pivotLeft90();
	else if (orientation == EAST) {
		// If has left wall, align with left wall
		if (useAlignment && hasNorthWall(curPosX, curPosY)) {
			pivotLeft90();
			align();
			pivotLeft90();
		}
		// Else if has right wall, align with right wall
		else if (useAlignment && hasSouthWall(curPosX, curPosY)) {
			pivotRight90();
			align();
			pivotRight90();
		}
		else {
			// Randomly pivot 180
			(millis()%2)? pivotLeft180():pivotRight180();
		}
	}
	else if (orientation == SOUTH)
		pivotRight90();

	orientation = WEST;
}

// Check if approaching front wall
bool approachingWall (void) {
	if ( (LFSensorT > approachThresholdL) && (RFSensorT > approachThresholdR) )
		return 1;
	else
		return 0;
}

float getStartSpeed(int32_t profile) {
	switch(profile) {
		case SS0: 	return straightSpeed;
		case SS90L: 	return curveLeft90SSProfile[speedRunProfile].startSpeed;
		case SS90R: 	return curveRight90SSProfile[speedRunProfile].startSpeed;
		case SS180L: 	return curveLeft180Profile[speedRunProfile].startSpeed;
		case SS180R: 	return curveRight180Profile[speedRunProfile].startSpeed;
		case SD45L: 	return curveLeft45SDProfile[speedRunProfile].startSpeed;
		case SD45R: 	return curveRight45SDProfile[speedRunProfile].startSpeed;
		case SD135L:	return curveLeft135SDProfile[speedRunProfile].startSpeed;
		case SD135R:	return curveRight135SDProfile[speedRunProfile].startSpeed;
		case DS45L:	return curveLeft45DSProfile[speedRunProfile].startSpeed;
		case DS45R:	return curveRight45DSProfile[speedRunProfile].startSpeed;
		case DS135L:	return curveLeft135DSProfile[speedRunProfile].startSpeed;
		case DS135R:	return curveRight135DSProfile[speedRunProfile].startSpeed;
		case DD90L:	return curveLeft90DDProfile[speedRunProfile].startSpeed;
		case DD90R:	return curveRight90DDProfile[speedRunProfile].startSpeed;
		case LEFT:	return curveLeft90Profile[searchProfile].startSpeed;
		case RIGHT:	return curveRight90Profile[searchProfile].startSpeed;
		default:	return 0;
	}
}

float getEndSpeed(int32_t profile) {
		switch(profile) {
		case SS0: 	return straightSpeed;
		case SS90L: 	return curveLeft90SSProfile[speedRunProfile].endSpeed;
		case SS90R: 	return curveRight90SSProfile[speedRunProfile].endSpeed;
		case SS180L: 	return curveLeft180Profile[speedRunProfile].endSpeed;
		case SS180R: 	return curveRight180Profile[speedRunProfile].endSpeed;
		case SD45L: 	return curveLeft45SDProfile[speedRunProfile].endSpeed;
		case SD45R: 	return curveRight45SDProfile[speedRunProfile].endSpeed;
		case SD135L:	return curveLeft135SDProfile[speedRunProfile].endSpeed;
		case SD135R:	return curveRight135SDProfile[speedRunProfile].endSpeed;
		case DS45L:	return curveLeft45DSProfile[speedRunProfile].endSpeed;
		case DS45R:	return curveRight45DSProfile[speedRunProfile].endSpeed;
		case DS135L:	return curveLeft135DSProfile[speedRunProfile].endSpeed;
		case DS135R:	return curveRight135DSProfile[speedRunProfile].endSpeed;
		case DD90L:	return curveLeft90DDProfile[speedRunProfile].endSpeed;
		case DD90R:	return curveRight90DDProfile[speedRunProfile].endSpeed;
		case LEFT:	return curveLeft90Profile[searchProfile].endSpeed;
		case RIGHT:	return curveRight90Profile[searchProfile].endSpeed;
		default:	return 0;
	}
}
