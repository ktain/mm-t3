#include "main.h"

uint16_t nextDir[300] = {0};
int32_t length[300] = {0};
int32_t tempDistance[SIZEX][SIZEY];
int32_t tempCell[SIZEX][SIZEY];

uint16_t pathSLR[300] = {0};
uint16_t path[300] = {0};

int32_t generatePathDiag(uint16_t targetX, uint16_t targetY) {
	if (generatePathSLR(targetX, targetY) == 0) {	// if error, return 0
		return 0;
	}

	// zero path array
	for (int32_t i = 0; i < 300; i++) {
		path[i] = 0;
	}

	uint32_t head = 0;
	uint32_t headSLR = 0;
	int32_t diagCount = 0;
	for (headSLR = 0; pathSLR[headSLR] != 0; headSLR++) {
		// printf("headSLR = %d\r\n", headSLR);
		// Stopping condition
		if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == 0) {

		}

		// Straight to straight 0
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == STRAIGHT) {
			uint32_t i;
			for (i = 0; pathSLR[headSLR + i] == STRAIGHT; i++);	// count straights
			path[head++] = SS0;
			path[head++] = i-1;
			headSLR += i-2;
		}

		// Straight to straight +90
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = SS90R;
			headSLR++;
		}

		// Straight to straight -90
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = SS90L;
			headSLR++;
		}

		// Straight to straight +180
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == RIGHT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = SS180R;
			headSLR += 2;
		}

		// Straight to straight -180
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == LEFT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = SS180L;
			headSLR += 2;
		}

		// Straight to diagonal +45
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == LEFT) {
			diagCount = countDiagonals(RIGHT, headSLR + 1);
			path[head++] = SD45R;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount;
		}

		// Straight to diagonal -45
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == RIGHT) {
			diagCount = countDiagonals(LEFT, headSLR + 1);
			path[head++] = SD45L;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount;
		}

		// Straight to diagonal +135
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == RIGHT && pathSLR[headSLR+3] == LEFT) {
			diagCount = countDiagonals(RIGHT, headSLR + 2);
			path[head++] = SD135R;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount + 1;
		}

		// Straight to diagonal -135
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == LEFT && pathSLR[headSLR+3] == RIGHT) {
			diagCount = countDiagonals(LEFT, headSLR + 2);
			path[head++] = SD135L;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount + 1;
		}

		// Diagonal to straight +45
		else if (pathSLR[headSLR] == LEFT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = DS45R;
			headSLR++;
		}

		// Diagonal to straight -45
		else if (pathSLR[headSLR] == RIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = DS45L;
			headSLR++;
		}

		// Diagonal to straight +135
		else if (pathSLR[headSLR] == LEFT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == RIGHT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = DS135R;
			headSLR += 2;
		}

		// Diagonal to straight -135
		else if (pathSLR[headSLR] == RIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == LEFT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = DS135L;
			headSLR += 2;
		}

		// Diagonal to diagonal +90
		else if (pathSLR[headSLR] == LEFT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == RIGHT && pathSLR[headSLR+3] == LEFT) {
			diagCount = countDiagonals(RIGHT, headSLR + 2);
			path[head++] = DD90R;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount + 1;
		}

		// Diagonal to diagonal -90
		else if (pathSLR[headSLR] == RIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == LEFT && pathSLR[headSLR+3] == RIGHT) {
			diagCount = countDiagonals(LEFT, headSLR + 2);
			path[head++] = DD90L;
			if (diagCount > 0) {
				path[head++] = DD0;
				path[head++] = diagCount;
			}
			headSLR += diagCount + 1;
		}
	}
	return 1;
}

int32_t generatePathSmooth(uint16_t targetX, uint16_t targetY) {
	if (generatePathSLR(targetX, targetY) == 0) {	// if error, return 0
		return 0;
	}

	// zero path array
	for (int32_t i = 0; i < 300; i++) {
		path[i] = 0;
	}

	uint32_t head = 0;
	uint32_t headSLR = 0;
	for (headSLR = 0; pathSLR[headSLR] != 0; headSLR++) {
		// Stopping condition
		if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == 0) {

		}

		// Straight to straight 0
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == STRAIGHT) {
			uint32_t i;
			for (i = 0; pathSLR[headSLR + i] == STRAIGHT; i++);	// count straights
			path[head++] = SS0;
			path[head++] = i-1;
			headSLR += i-2;
		}

		// Straight to straight +90
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = SS90R;
			headSLR += 2;
		}

		// Straight to straight -90
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == STRAIGHT) {
			path[head++] = SS90L;
			headSLR += 2;
		}

		// Straight to straight +180
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == RIGHT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = SS180R;
			headSLR += 2;
		}

		// Straight to straight -180
		else if (pathSLR[headSLR] == STRAIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == LEFT && pathSLR[headSLR+3] == STRAIGHT) {
			path[head++] = SS180L;
			headSLR += 2;
		}

		// Left
		else if (pathSLR[headSLR] == LEFT) {
			path[head++] = LEFT;
		}

		// Right
		else if (pathSLR[headSLR] == RIGHT) {
			path[head++] = RIGHT;
		}

		// Straight 0
		else if (pathSLR[headSLR] == STRAIGHT) {
			path[head++] = SS0;
			path[head++] = 0;
			headSLR++;
		}
	}
	
	/*
	for (int i = 0; pathSLR[i]; i++)
		printf("path[i] = %d\r\n", path[i]);
	*/

	return 1;
}


int32_t speedRunDiag(uint16_t targetX, uint16_t targetY) {
	if (generatePathDiag(targetX, targetY) == 0) {	// if error, return 0
		return 0;
	}

	uint32_t i = 0;
	enableMotorControl();
	// Initial case
	if (curPosX == 0 && curPosY == 0 && orientation == NORTH) {
		moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance, straightSpeed, getStartSpeed(path[i]));
	}
	for (i = 0; path[i] != 0; i++) {
		if (path[i] == SS0) {
			i++;
			moveStraight(path[i], straightSpeed, getStartSpeed(path[i+1]));
		}
		else if (path[i] == SS90L) {
			curveLeft90SS();
		}
		else if (path[i] == SS90R) {
			curveRight90SS();
		}
		else if (path[i] == SS180L) {
			curveLeft180SS();
		}
		else if (path[i] == SS180R) {
			curveRight180SS();
		}
		else if (path[i] == SD45L) {
			curveLeft45SD();
		}
		else if (path[i] == SD45R) {
			curveRight45SD();
		}
		else if (path[i]== SD135L) {
			curveLeft135SD();
		}
		else if (path[i] == SD135R) {
			curveRight135SD();
		}
		else if (path[i]== DS45L) {
			curveLeft45DS();
		}
		else if (path[i] == DS45R) {
			curveRight45DS();
		}
		else if (path[i]== DS135L) {
			curveLeft135DS();
		}
		else if (path[i]== DS135R) {
			curveRight135DS();
		}
		else if (path[i]== DD90L) {
			curveLeft90DD();
		}
		else if (path[i] == DD90R) {
			curveRight90DD();
		}
		else if (path[i]== DD0) {
			i++;
			moveDiagonal(path[i], diagSpeed, getStartSpeed(path[i+1]));
		}
	}

	// Detect overshoot
	int32_t overshoot = (getLeftEncCount() + getRightEncCount())/2;
	decX = 10;
	targetSpeedX = 0;
	delay_ms(500);
	
	// Update position
	curPosX = targetX;
	curPosY = targetY;

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		overshoot = 0;
		align();
	}
	else {
		overshoot = (getLeftEncCount() + getRightEncCount())/2 - overshoot;
	}
	
	// If has left wall, align with left wall
	if (useAlignment && ((orientation == NORTH && hasWestWall(curPosX, curPosY)) || (orientation == EAST && hasNorthWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasEastWall(curPosX, curPosY)) || (orientation == WEST && hasSouthWall(curPosX, curPosY)))) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	// Else if has right wall, align with right wall
	else if (useAlignment && ((orientation == NORTH && hasEastWall(curPosX, curPosY)) || (orientation == EAST && hasSouthWall(curPosX, curPosY)) ||
		 (orientation == SOUTH && hasWestWall(curPosX, curPosY)) || (orientation == WEST && hasNorthWall(curPosX, curPosY)))) {
		pivotRight90();
		align();
		pivotRight90();
	}
	// Else, no alignment, rotate right 180
	else {
		// Turn around
		pivotRight180();
	}
	
	delay_ms(pivotTurnDelay);

	// Move to center of cell to correct overshoot
	if (overshoot > 0) {
		moveStraight(overshoot/cellDistance, 0.2, stopSpeed);
		delay_ms(200);
	}
	
	if (orientation == NORTH)
		orientation = SOUTH;
	else if (orientation == EAST)
		orientation = WEST;
	else if (orientation == SOUTH)
		orientation = NORTH;
	else if (orientation == WEST)
		orientation = EAST;

	disableMotorControl();
	return 1;
}

int32_t speedRunSmooth(uint16_t targetX, uint16_t targetY) {
	if (generatePathSmooth(targetX, targetY) == 0) {	// if error, return 0
		return 0;
	}

	uint32_t i = 0;
	enableMotorControl();
	// Initial case
	if (curPosX == 0 && curPosY == 0 && orientation == NORTH) {
		if (path[i] == LEFT || path[i] == RIGHT)
			moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5f, straightSpeed, getStartSpeed(path[i]));
		else
			moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance, straightSpeed, getStartSpeed(path[i]));
		
		if (orientation == NORTH)
			curPosY++;
		else if (orientation == EAST)
			curPosX++;
		else if (orientation == SOUTH)
			curPosY--;
		else if (orientation == WEST)
			curPosX--;
	}
	for (i = 0; path[i] != 0; i++) {
		if (path[i] == SS0) {
			i++;	// skip distance value
			if (path[i+1] == LEFT || path[i+1] == RIGHT)
				moveStraight((float)path[i] + 0.5f, straightSpeed, getStartSpeed(path[i+1]));
			else 
				moveStraight(path[i], straightSpeed, getStartSpeed(path[i+1]));
			
			if (orientation == NORTH)
				curPosY += path[i+1];
			else if (orientation == EAST)
				curPosX += path[i+1];
			else if (orientation == SOUTH)
				curPosY -= path[i+1];
			else if (orientation == WEST)
				curPosX -= path[i+1];
		}
		else if (path[i] == SS90L) {
			curveLeft90SS();
			
			if (orientation == NORTH) {
				curPosX -= 2;
				orientation = WEST;
			}
			else if (orientation == EAST) {
				curPosY += 2;
				orientation = NORTH;
			}
			else if (orientation == SOUTH) {
				curPosX += 2;
				orientation = EAST;
			}
			else if (orientation == WEST) {
				curPosY -= 2;
				orientation = SOUTH;
			}
		}
		else if (path[i] == SS90R) {
			curveRight90SS();
			
			if (orientation == NORTH) {
				curPosX += 2;
				orientation = EAST;
			}
			else if (orientation == EAST) {
				curPosY -= 2;
				orientation = SOUTH;
			}
			else if (orientation == SOUTH) {
				curPosX -= 2;
				orientation = WEST;
			}
			else if (orientation == WEST) {
				curPosY += 2;
				orientation = NORTH;
			}
		}
		else if (path[i] == SS180L) {
			curveLeft180SS();
			
			if (orientation == NORTH) {
				curPosX--;
				curPosY -= 2;
				orientation = SOUTH;
			}
			else if (orientation == EAST) {
				curPosX -= 2;
				curPosY++;
				orientation = WEST;
			}
			else if (orientation == SOUTH) {
				curPosX++;
				curPosY += 2;
				orientation = NORTH;
			}
			else if (orientation == WEST) {
				curPosX += 2;
				curPosY--;
				orientation = EAST;
			}
		}
		else if (path[i] == SS180R) {
			curveRight180SS();
			
			if (orientation == NORTH) {
				curPosX++;
				curPosY -= 2;
				orientation = SOUTH;
			}
			else if (orientation == EAST) {
				curPosX -= 2;
				curPosY--;
				orientation = WEST;
			}
			else if (orientation == SOUTH) {
				curPosX--;
				curPosY += 2;
				orientation = NORTH;
			}
			else if (orientation == WEST) {
				curPosX += 2;
				curPosY++;
				orientation = EAST;
			}
		}
		else if (path[i] == LEFT) {
			curveLeft90();
			
			if (path[i+1] != LEFT && path[i+1] != RIGHT)
				moveStraight(0.5, straightSpeed, getStartSpeed(path[i+1]));
			
			if (orientation == NORTH) {
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == EAST) {
				curPosY++;
				orientation = NORTH;
			}
			else if (orientation == SOUTH) {
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == WEST) {
				curPosY--;
				orientation = SOUTH;
			}
		}
		else if (path[i] == RIGHT) {
			curveRight90();
			
			if (path[i+1] != LEFT && path[i+1] != RIGHT)
				moveStraight(0.5, straightSpeed, getStartSpeed(path[i+1]));
			
			if (orientation == NORTH) {
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == EAST) {
				curPosY--;
				orientation = SOUTH;
			}
			else if (orientation == SOUTH) {
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == WEST) {
				curPosY++;
				orientation = NORTH;
			}
		}
	}
	
	// Detect overshoot
	int32_t overshoot = (getLeftEncCount() + getRightEncCount())/2;
	decX = 10;
	targetSpeedX = 0;
	delay_ms(500);
	
	// Update position
	curPosX = targetX;
	curPosY = targetY;

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		overshoot = 0;
		align();
	}
	else {
		overshoot = (getLeftEncCount() + getRightEncCount())/2 - overshoot;
	}
	
	// If has left wall, align with left wall
	if (useAlignment && ((orientation == NORTH && hasWestWall(curPosX, curPosY)) || (orientation == EAST && hasNorthWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasEastWall(curPosX, curPosY)) || (orientation == WEST && hasSouthWall(curPosX, curPosY)))) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	// Else if has right wall, align with right wall
	else if (useAlignment && ((orientation == NORTH && hasEastWall(curPosX, curPosY)) || (orientation == EAST && hasSouthWall(curPosX, curPosY)) ||
		 (orientation == SOUTH && hasWestWall(curPosX, curPosY)) || (orientation == WEST && hasNorthWall(curPosX, curPosY)))) {
		pivotRight90();
		align();
		pivotRight90();
	}
	// Else, no alignment, rotate right 180
	else {
		// Turn around
		pivotRight180();
	}
	
	delay_ms(pivotTurnDelay);

	// Move to center of cell to correct overshoot
	if (overshoot > 0) {
		moveStraight(overshoot/cellDistance, 0.2, stopSpeed);
		delay_ms(200);
	}
	
	if (orientation == NORTH)
		orientation = SOUTH;
	else if (orientation == EAST)
		orientation = WEST;
	else if (orientation == SOUTH)
		orientation = NORTH;
	else if (orientation == WEST)
		orientation = EAST;

	disableMotorControl();
	return 1;
}

int32_t speedRunClassic(uint16_t targetX, uint16_t targetY){
	if (generatePathNESW(targetX, targetY) == 0) {
		return 0;
	}
	enableMotorControl();

	for (uint32_t i = 0; length[i] != 0; i++) {
		// First straight
		if (i == 0) {
			if (curPosX == 0 && curPosY == 0 && orientation == NORTH)
				moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + length[i] - 0.5f, straightSpeed, turnSpeed);
			else
				moveStraight(length[i] - 0.5f, straightSpeed, turnSpeed);

			if (orientation == NORTH)
				curPosY += length[i];
			else if (orientation == EAST)
				curPosX += length[i];
			else if (orientation == SOUTH)
				curPosY -= length[i];
			else if (orientation == WEST)
				curPosX -= length[i];
		}
		// Last straight
		else if (length[i+1] == 0) {
			moveStraight(length[i] - (1 - (0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance)), straightSpeed, stopSpeed);
			if (orientation == NORTH)
				curPosY += length[i] - 1;
			else if (orientation == EAST)
				curPosX += length[i] - 1;
			else if (orientation == SOUTH)
				curPosY -= length[i] - 1;
			else if (orientation == WEST)
				curPosX -= length[i] - 1;
		}
		// Intermediate straight
		else {
			moveStraight(length[i] - 1, straightSpeed, turnSpeed);
			if (orientation == NORTH)
				curPosY += length[i] - 1;
			else if (orientation == EAST)
				curPosX += length[i] - 1;
			else if (orientation == SOUTH)
				curPosY -= length[i] - 1;
			else if (orientation == WEST)
				curPosX -= length[i] - 1;
		}

		if (length[i+1] == 0)
			break;

		// Curve turn
		if (nextDir[i] == NORTH)
			moveN();
		else if (nextDir[i] == EAST)
			moveE();
		else if (nextDir[i] == SOUTH)
			moveS();
		else if (nextDir[i] == WEST)
			moveW();
	}

	delay_ms(200);

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		align();
	}
	
	// If has left wall, align with left wall
	if (useAlignment && ((orientation == NORTH && hasWestWall(curPosX, curPosY)) || (orientation == EAST && hasNorthWall(curPosX, curPosY)) ||
	   (orientation == SOUTH && hasEastWall(curPosX, curPosY)) || (orientation == WEST && hasSouthWall(curPosX, curPosY)))) {
		pivotLeft90();
		align();
		pivotLeft90();
	}
	// Else if has right wall, align with right wall
	else if (useAlignment && ((orientation == NORTH && hasEastWall(curPosX, curPosY)) || (orientation == EAST && hasSouthWall(curPosX, curPosY)) ||
		 (orientation == SOUTH && hasWestWall(curPosX, curPosY)) || (orientation == WEST && hasNorthWall(curPosX, curPosY)))) {
		pivotRight90();
		align();
		pivotRight90();
	}
	// Else, no alignment, rotate right 180
	else {
		pivotRight180();
	}

	if (orientation == NORTH)
		orientation = SOUTH;
	else if (orientation == EAST)
		orientation = WEST;
	else if (orientation == SOUTH)
		orientation = NORTH;
	else if (orientation == WEST)
		orientation = EAST;

	disableMotorControl();
	return 1;
}


int32_t generatePathNESW(uint16_t targetX, uint16_t targetY) {
	uint32_t count = 0;

	// zero nextDir and length array
	for (int32_t i = 0; i < 300; i++) {
		nextDir[i] = 0;
		length[i] = 0;
	}

	// Store copy of distance values
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			tempDistance[i][j] = distance[i][j];
		}
	}

	// Store copy of wall info
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			tempCell[i][j] = cell[i][j];
		}
	}

	// Simulate current position and orientation
	uint16_t simPosX = curPosX;
	uint16_t simPosY = curPosY;
	uint16_t simOrientation = orientation;

	// Block off untraced routes
	closeUntracedCells();
	updateDistancesComplete(targetX, targetY, targetX, targetY, targetX, targetY, targetX, targetY);

	uint32_t i;
	for (i = 0; !(simPosX == targetX && simPosY == targetY); i++) {
		if (simOrientation == NORTH) {
			while (!hasNorthWall(simPosX, simPosY) && (distance[simPosX][simPosY+1] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX, simPosY+1)) {
				simPosY++;
				count++;
			}
		}
		else if (simOrientation == EAST) {
			while (!hasEastWall(simPosX, simPosY) && (distance[simPosX+1][simPosY] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX+1, simPosY)) {
				simPosX++;
				count++;
			}
		}
		else if (simOrientation == SOUTH) {
			while (!hasSouthWall(simPosX, simPosY) && (distance[simPosX][simPosY-1] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX, simPosY-1)) {
				simPosY--;
				count++;
			}
		}
		else if (simOrientation == WEST) {
			while (!hasWestWall(simPosX, simPosY) && (distance[simPosX-1][simPosY] == distance[simPosX][simPosY] - 1) && hasTrace(simPosX-1, simPosY)) {
				simPosX--;
				count++;
			}
		}
		//Error check
		if (count == 0) {
			// Restore distance values
			for (uint32_t i = 0; i < SIZEX; i++) {
				for (uint32_t j = 0; j < SIZEY; j++) {
					distance[i][j] = tempDistance[i][j];
				}
			}

			// Restore cell data
			for (uint32_t i = 0; i < SIZEX; i++) {
				for (uint32_t j = 0; j < SIZEY; j++) {
					cell[i][j] = tempCell[i][j];
				}
			}
			return 0;
		}
		length[i] = count;
		nextDir[i] = getNextDirection(simPosX, simPosY);
		simOrientation = nextDir[i];
		count = 0;
	}

	// Encode stopping condition
	length[i] = 0;
	nextDir[i] = 0;

	// Restore distance values
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			distance[i][j] = tempDistance[i][j];
		}
	}

	// Restore cell data
	for (uint32_t i = 0; i < SIZEX; i++) {
		for (uint32_t j = 0; j < SIZEY; j++) {
			cell[i][j] = tempCell[i][j];
		}
	}

	return 1;
}

// Returns next direction to move in
uint16_t getNextDirection(uint16_t xPos, uint16_t yPos) {
	uint32_t curDist = distance[xPos][yPos];
	uint16_t nextDir = 0;
	uint32_t distN = MAX_DIST;
	uint32_t distE = MAX_DIST;
	uint32_t distS = MAX_DIST;
	uint32_t distW = MAX_DIST;

	if (yPos < SIZEY-1)
		distN = distance[xPos][yPos+1];
	if (xPos < SIZEX-1)
		distE = distance[xPos+1][yPos];
	if (yPos > 0)
		distS = distance[xPos][yPos-1];
	if (xPos > 0)
		distW = distance[xPos-1][yPos];

	if (!hasNorthWall(xPos, yPos) && (distN == curDist-1)) {
		nextDir = NORTH;
	}
	else if (!hasEastWall(xPos, yPos) && (distE == curDist-1)) {
		nextDir = EAST;
	}
	else if (!hasSouthWall(xPos, yPos) && (distS == curDist-1)) {
		nextDir = SOUTH;
	}
	else if (!hasWestWall(xPos, yPos) && (distW == curDist-1)) {
		nextDir = WEST;
	}

	return nextDir;
}

void closeUntracedCells(void) {
	for (uint32_t x = 0; x < SIZEX; x++) {
		for (uint32_t y = 0; y < SIZEY; y++) {
			if (!hasTrace(x, y)) {
				placeWall(x, y, NORTH);
				placeWall(x, y, EAST);
				placeWall(x, y, SOUTH);
				placeWall(x, y, WEST);
			}
		}
	}
}

int32_t generatePathSLR(uint16_t targetX, uint16_t targetY) {
	// Path translation
	if (generatePathNESW(targetX, targetY) == 0) {	// if error, return 0
		return 0;
	}

	// zero pathSLR array
	for (int32_t i = 0; i < 300; i++) {
		pathSLR[i] = 0;
	}

	uint32_t i, j;
	uint32_t head = 0;
	for (i = 0; length[i] != 0; i++) {

		// first case
		if (i == 0) {
			for (j = 0; j < length[i]; j++) {
				pathSLR[head + j] = STRAIGHT;
			}
			head += j;
			if (nextDir[i] == NORTH && orientation == EAST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == NORTH && orientation == WEST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && orientation == NORTH) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && orientation == SOUTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == SOUTH && orientation == EAST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == SOUTH && orientation == WEST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && orientation == NORTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && orientation == SOUTH) {
				pathSLR[head] = RIGHT;
			}
		}
		// intermediate case
		else {
			for (j = 0; j < length[i]-1; j++) {
				pathSLR[head + j] = STRAIGHT;
			}
			head += j;
			if (nextDir[i] == NORTH && nextDir[i-1] == EAST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == NORTH && nextDir[i-1] == WEST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && nextDir[i-1] == NORTH) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == EAST && nextDir[i-1] == SOUTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == SOUTH && nextDir[i-1] == EAST) {
				pathSLR[head] = RIGHT;
			}
			else if (nextDir[i] == SOUTH && nextDir[i-1] == WEST) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && nextDir[i-1] == NORTH) {
				pathSLR[head] = LEFT;
			}
			else if (nextDir[i] == WEST && nextDir[i-1] == SOUTH) {
				pathSLR[head] = RIGHT;
			}
		}
		head++;
	}

	// last case
	head--;
	pathSLR[head] = STRAIGHT;

	return 1;
}


int32_t countDiagonals(uint32_t startDir, uint32_t headSLR) {
	// +45 -135
	if (startDir == RIGHT && pathSLR[headSLR] == RIGHT && pathSLR[headSLR+1] == LEFT && pathSLR[headSLR+2] == RIGHT)
		return countDiagonals(LEFT, headSLR + 1) + 1;
	// -45 +135
	else if (startDir == LEFT && pathSLR[headSLR] == LEFT && pathSLR[headSLR+1] == RIGHT && pathSLR[headSLR+2] == LEFT)
		return countDiagonals(RIGHT, headSLR + 1) + 1;

	return 0;
}
