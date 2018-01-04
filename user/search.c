#include "main.h"

bool useBacktracking;
bool updateAdjacentCells;
int32_t pivotTurnDelay;
int16_t destinationX, destinationY;

int32_t detectWallOvershoot = 0;

struct cellStack {
	uint16_t x;
	uint16_t y;
};

struct cellStack distStack[2*MAX_DIST];
uint32_t top_index = 0;

struct cellStack visitStack[2*MAX_DIST];
uint32_t top_index_visit = 0;


// Flood search to target position using curve turns
int32_t floodSearch(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4) {
	enableMotorControl();
	updateDistancesComplete(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);
	placeTrace(curPosX, curPosY);
	
	if (curPosX == 0 && curPosY == 0 && orientation == NORTH) {
		moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5f, searchSpeed, searchSpeed);
	}
	else {
		moveStraight(0.5, searchSpeed, searchSpeed);
	}
	
	if (orientation == NORTH)
		curPosY++;
	else if (orientation == EAST)
		curPosX++;
	else if (orientation == SOUTH)
		curPosY--;
	else if (orientation == WEST)
		curPosX--;
	
	detectWalls();
	
	if (useBacktracking)
		simulateStraight(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);
		
	while( !atTarget(targetX1, targetY1) && !atTarget(targetX2, targetY2) && !atTarget(targetX3, targetY3) && !atTarget(targetX4, targetY4) ) {
		//Error check
		if (distance[curPosX][curPosY] >= MAX_DIST) {
			targetSpeedX = 0;
			disableMotorControl();
			setLeftPwm(0);
			setRightPwm(0);
			display("Err floodsearch");
			return 0;
		}
		
		performNextMove();
		detectWalls();
		
		if (useBacktracking)
			simulateStraight(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);
	}
	detectWalls();
	
	if (orientation == NORTH && !hasNorthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY + 1)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosY++;
		detectWalls();
	}
	else if (orientation == EAST && !hasEastWall(curPosX, curPosY) && !hasTrace(curPosX + 1, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosX++;
		detectWalls();
	}
	else if (orientation == SOUTH && !hasSouthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY - 1)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosY--;
		detectWalls();
	}
	else if (orientation == WEST && !hasWestWall(curPosX, curPosY) && !hasTrace(curPosX - 1, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosX--;
		detectWalls();
	}

	
	if (!atTarget(0, 0)) {
		destinationX = curPosX;
		destinationY = curPosY;
	}

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) ||   (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		moveStraight(0.5f, searchSpeed, stopSpeed);
		delay_ms(alignTime);
		align();
	}
	else {
		moveStraight(0.5f, searchSpeed, stopSpeed);
		delay_ms(pivotTurnDelay);
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
	// Else rotate right 180
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

void performNextMove() {

	uint16_t nextMove;
	uint32_t distN;
	uint32_t distE;
	uint32_t distS;
	uint32_t distW;

	// Get distances around current block
	distN = hasNorthWall(curPosX, curPosY)? MAX_DIST : distance[curPosX][curPosY+1];
	distE = hasEastWall(curPosX, curPosY)? MAX_DIST : distance[curPosX+1][curPosY];
	distS = hasSouthWall(curPosX, curPosY)? MAX_DIST : distance[curPosX][curPosY-1];
	distW = hasWestWall(curPosX, curPosY)? MAX_DIST : distance[curPosX-1][curPosY];

	// Decide next movement
	// 1. Pick the shortest route
	if ( (distN < distE) && (distN < distS) && (distN < distW) )
		nextMove = NORTH;
	else if ( (distE < distN) && (distE < distS) && (distE < distW) )
		nextMove = EAST;
	else if ( (distS < distE) && (distS < distN) && (distS < distW) )
		nextMove = SOUTH;
	else if ( (distW < distE) && (distW < distS) && (distW < distN) )
		nextMove = WEST;

	// Else, there are multiple equally short routes
	else {

		// 2. Choose shortest untraced route prioritizing going straight
		if ( (orientation == NORTH) && !hasNorthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY+1) && (distN <= distE) && (distN <= distS) && (distN <= distW) )
			nextMove = NORTH;
		else if ( (orientation == EAST) && !hasEastWall(curPosX, curPosY) && !hasTrace(curPosX+1, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW) )
			nextMove = EAST;
		else if ( (orientation == SOUTH) && !hasSouthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY-1) && (distS <= distE) && (distS <= distN) && (distS <= distW) )
			nextMove = SOUTH;
		else if ( (orientation == WEST) && !hasWestWall(curPosX, curPosY) && !hasTrace(curPosX-1,curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN) )
			nextMove = WEST;
		
		// 3. Else, choose shortest untraced route prioritizing N > E > S > W
		else if ( !hasNorthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY+1) && (distN <= distE) && (distN <= distS) && (distN <= distW))
			nextMove = NORTH;
		else if ( !hasEastWall(curPosX, curPosY) && !hasTrace(curPosX+1, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW))
			nextMove = EAST;
		else if ( !hasSouthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY-1) && (distS <= distE) && (distS <= distN) && (distS <= distW))
			nextMove = SOUTH;
		else if ( !hasWestWall(curPosX, curPosY) && !hasTrace(curPosX-1,curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN))
			nextMove = WEST;  
		
		// 4. Else, all routes have traces, choose shortest route prioritizing going straight
		if ( (orientation == NORTH) && !hasNorthWall(curPosX, curPosY) && (distN <= distE) && (distN <= distS) && (distN <= distW) )
			nextMove = NORTH;
		else if ( (orientation == EAST) && !hasEastWall(curPosX, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW) )
			nextMove = EAST;
		else if ( (orientation == SOUTH) && !hasSouthWall(curPosX, curPosY) && (distS <= distE) && (distS <= distN) && (distS <= distW) )
			nextMove = SOUTH;
		else if ( (orientation == WEST) && !hasWestWall(curPosX, curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN) )
			nextMove = WEST;
		
		// 5. Else, choose shortest route prioritizing N > E > S > W
		else if ( !hasNorthWall(curPosX, curPosY) && (distN <= distE) && (distN <= distS) && (distN <= distW))
			nextMove = NORTH;
		else if ( !hasEastWall(curPosX, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW))
			nextMove = EAST;
		else if ( !hasSouthWall(curPosX, curPosY) && (distS <= distE) && (distS <= distN) && (distS <= distW))
			nextMove = SOUTH;
		else if ( !hasWestWall(curPosX, curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN))
			nextMove = WEST;  
		
		// 6. Else, go straight if possible
		else if ( orientation == NORTH && !hasNorthWall(curPosX, curPosY) )
			nextMove = NORTH;
		else if ( orientation == EAST && !hasEastWall(curPosX, curPosY) )
			nextMove = EAST;
		else if ( orientation == SOUTH && !hasSouthWall(curPosX, curPosY) )
			nextMove = SOUTH;
		else if ( orientation == WEST && !hasWestWall(curPosX, curPosY) )
			nextMove = WEST;
		
		// 7. Else, prioritize N > E > S > W
		else if (!hasNorthWall(curPosX, curPosY))
			nextMove = NORTH;
		else if (!hasEastWall(curPosX, curPosY))
			nextMove = EAST;
		else if (!hasSouthWall(curPosX, curPosY))
			nextMove = SOUTH;
		else if (!hasWestWall(curPosX, curPosY))
			nextMove = WEST;
	}

	// Perform next move
	if (nextMove == NORTH)
		moveN();
	else if (nextMove == EAST)
		moveE();
	else if (nextMove == SOUTH)
		moveS();
	else if (nextMove == WEST)
		moveW();
		
	// Reset detect wall overshoot
	detectWallOvershoot = 0;
}



void simulateStraight(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4) {
	uint16_t xPos = curPosX;
	uint16_t yPos = curPosY;
	uint32_t count = 0;

	if (orientation == NORTH) {
		while (!hasNorthWall(xPos, yPos) && hasTrace(xPos, yPos+1) && (distance[xPos][yPos+1] ==  distance[xPos][yPos] - 1) && 
		       !(xPos == targetX1 && yPos == targetY1) && !(xPos == targetX2 && yPos == targetY2) &&
		       !(xPos == targetX3 && yPos == targetY3) && !(xPos == targetX4 && yPos == targetY4)) {
			count++;
			yPos++;
		}
		// if 1 more cell is also a target cell, move 1 more cell
		if (!hasNorthWall(xPos, yPos) && hasTrace(xPos, yPos+1) && ((xPos == targetX1 && yPos+1 == targetY1) || (xPos == targetX2 && yPos+1 == targetY2) ||
		   (xPos == targetX3 && yPos+1 == targetY3) || (xPos == targetX4 && yPos+1 == targetY4))) {
			count++;
			yPos++;
		}
	}

	else if (orientation == EAST) {
		while (!hasEastWall(xPos, yPos) && hasTrace(xPos+1, yPos) && (distance[xPos+1][yPos] == distance[xPos][yPos] - 1) && 
		       !(xPos == targetX1 && yPos == targetY1) && !(xPos == targetX2 && yPos == targetY2) &&
		       !(xPos == targetX3 && yPos == targetY3) && !(xPos == targetX4 && yPos == targetY4)) {
			count++;
			xPos++;
		}
		// if 1 more cell is also a target cell, move 1 more cell
		if (!hasEastWall(xPos, yPos) && hasTrace(xPos+1, yPos) && ((xPos+1 == targetX1 && yPos == targetY1) || (xPos+1 == targetX2 && yPos == targetY2) ||
		   (xPos+1 == targetX3 && yPos == targetY3) || (xPos+1 == targetX4 && yPos == targetY4))) {
			count++;
			xPos++;
		}
	}

	else if (orientation == SOUTH) {
		while (!hasSouthWall(xPos, yPos) && hasTrace(xPos, yPos-1) && (distance[xPos][yPos-1] == distance[xPos][yPos] - 1) && 
		       !(xPos == targetX1 && yPos == targetY1) && !(xPos == targetX2 && yPos == targetY2) &&
		       !(xPos == targetX3 && yPos == targetY3) && !(xPos == targetX4 && yPos == targetY4)) {
			count++;
			yPos--;
		}
		// if 1 more cell is also a target cell, move 1 more cell
		if (!hasSouthWall(xPos, yPos) && hasTrace(xPos, yPos-1) && ((xPos == targetX1 && yPos-1 == targetY1) || (xPos == targetX2 && yPos-1 == targetY2) ||
		   (xPos == targetX3 && yPos-1 == targetY3) || (xPos == targetX4 && yPos-1 == targetY4))) {
			count++;
			yPos--;
		}
	}

	else if (orientation == WEST) {
		while(!hasWestWall(xPos, yPos) && hasTrace(xPos-1, yPos) && (distance[xPos-1][yPos] == distance[xPos][yPos] - 1 ) && 
		       !(xPos == targetX1 && yPos == targetY1) && !(xPos == targetX2 && yPos == targetY2) &&
		       !(xPos == targetX3 && yPos == targetY3) && !(xPos == targetX4 && yPos == targetY4)) {
			count++;
			xPos--;
		}
		// if 1 more cell is also a target cell, move 1 more cell
		if (!hasWestWall(xPos, yPos) && hasTrace(xPos-1, yPos) && ((xPos-1 == targetX1 && yPos == targetY1) || (xPos-1 == targetX2 && yPos == targetY2) ||
		   (xPos-1 == targetX3 && yPos == targetY3) || (xPos-1 == targetX4 && yPos == targetY4))) {
			count++;
			xPos--;
		}
	}
	
	moveStraight(count, traceSpeed, searchSpeed);
	
	curPosX = xPos;
	curPosY = yPos;
}


// Flood search to target position using pivot turns
int32_t floodSearchPivot(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4) {
	enableMotorControl();
	updateDistancesComplete(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);
	placeTrace(curPosX, curPosY);

	if (curPosX == 0 && curPosY == 0 && orientation == NORTH) {
		moveStraight(mm_to_counts(84 - centerToBackDist + pivotOffsetBefore)/cellDistance + 0.5f, searchSpeed, searchSpeed);
	}
	else {
		moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, searchSpeed, searchSpeed);
	}

	if (orientation == NORTH)
		curPosY++;
	else if (orientation == EAST)
		curPosX++;
	else if (orientation == SOUTH)
		curPosY--;
	else if (orientation == WEST)
		curPosX--;

	detectWalls();
	
	if (useBacktracking)
		simulateStraight(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);

	while( !atTarget(targetX1, targetY1) && !atTarget(targetX2, targetY2) && !atTarget(targetX3, targetY3) && !atTarget(targetX4, targetY4)) {
		//Error check
		if (distance[curPosX][curPosY] >= MAX_DIST) {
			targetSpeedX = 0;
			delay_ms(500);
			disableMotorControl();
			setLeftPwm(0);
			setRightPwm(0);
			display("Err floodSearchPivot");
			return 0;
		}
		
		performNextMovePivot();
		detectWalls();
		
		if (useBacktracking)
			simulateStraight(targetX1, targetY1, targetX2, targetY2, targetX3, targetY3, targetX4, targetY4);
		
	}
	detectWalls();
	
	if (orientation == NORTH && !hasNorthWall(curPosX, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosY++;
		detectWalls();
	}
	else if (orientation == EAST && !hasEastWall(curPosX, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosX++;
		detectWalls();
	}
	else if (orientation == SOUTH && !hasSouthWall(curPosX, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosY--;
		detectWalls();
	}
	else if (orientation == WEST && !hasWestWall(curPosX, curPosY)) {
		moveStraight(1, searchSpeed, searchSpeed);
		curPosX--;
		detectWalls();
	}

	if (!atTarget(0,0)) {
		destinationX = curPosX;
		destinationY = curPosY;
	}

	// If has front wall, align
	if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
		moveStraight(0.5f, searchSpeed, stopSpeed);
		delay_ms(alignTime);
		align();
	}
	else {
		moveStraight(0.5f, searchSpeed, stopSpeed);
		delay_ms(pivotTurnDelay);
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
	// Else rotate right 180
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

void performNextMovePivot() {

	uint16_t nextMove;
	uint32_t distN;
	uint32_t distE;
	uint32_t distS;
	uint32_t distW;

	// Get distances around current block
	distN = hasNorthWall(curPosX, curPosY)? MAX_DIST : distance[curPosX][curPosY+1];
	distE = hasEastWall(curPosX, curPosY)? MAX_DIST : distance[curPosX+1][curPosY];
	distS = hasSouthWall(curPosX, curPosY)? MAX_DIST : distance[curPosX][curPosY-1];
	distW = hasWestWall(curPosX, curPosY)? MAX_DIST : distance[curPosX-1][curPosY];

	// Decide next movement
	// 1. Pick the shortest route
	if ( (distN < distE) && (distN < distS) && (distN < distW) )
		nextMove = NORTH;
	else if ( (distE < distN) && (distE < distS) && (distE < distW) )
		nextMove = EAST;
	else if ( (distS < distE) && (distS < distN) && (distS < distW) )
		nextMove = SOUTH;
	else if ( (distW < distE) && (distW < distS) && (distW < distN) )
		nextMove = WEST;

	// Else, there are multiple equally short routes
	else {

		// 2. Choose shortest untraced route prioritizing going straight
		if ( (orientation == NORTH) && !hasNorthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY+1) && (distN <= distE) && (distN <= distS) && (distN <= distW) )
			nextMove = NORTH;
		else if ( (orientation == EAST) && !hasEastWall(curPosX, curPosY) && !hasTrace(curPosX+1, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW) )
			nextMove = EAST;
		else if ( (orientation == SOUTH) && !hasSouthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY-1) && (distS <= distE) && (distS <= distN) && (distS <= distW) )
			nextMove = SOUTH;
		else if ( (orientation == WEST) && !hasWestWall(curPosX, curPosY) && !hasTrace(curPosX-1,curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN) )
			nextMove = WEST;

		// 3. Else, choose shortest untraced route prioritizing N > E > S > W
		else if ( !hasNorthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY+1) && (distN <= distE) && (distN <= distS) && (distN <= distW))
			nextMove = NORTH;
		else if ( !hasEastWall(curPosX, curPosY) && !hasTrace(curPosX+1, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW))
			nextMove = EAST;
		else if ( !hasSouthWall(curPosX, curPosY) && !hasTrace(curPosX, curPosY-1) && (distS <= distE) && (distS <= distN) && (distS <= distW))
			nextMove = SOUTH;
		else if ( !hasWestWall(curPosX, curPosY) && !hasTrace(curPosX-1,curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN))
			nextMove = WEST;  

		// 4. Else, all routes have traces, choose shortest route prioritizing going straight
		if ( (orientation == NORTH) && !hasNorthWall(curPosX, curPosY) && (distN <= distE) && (distN <= distS) && (distN <= distW) )
			nextMove = NORTH;
		else if ( (orientation == EAST) && !hasEastWall(curPosX, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW) )
			nextMove = EAST;
		else if ( (orientation == SOUTH) && !hasSouthWall(curPosX, curPosY) && (distS <= distE) && (distS <= distN) && (distS <= distW) )
			nextMove = SOUTH;
		else if ( (orientation == WEST) && !hasWestWall(curPosX, curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN) )
			nextMove = WEST;

		// 5. Else, choose shortest route prioritizing N > E > S > W
		else if ( !hasNorthWall(curPosX, curPosY) && (distN <= distE) && (distN <= distS) && (distN <= distW))
			nextMove = NORTH;
		else if ( !hasEastWall(curPosX, curPosY) && (distE <= distN) && (distE <= distS) && (distE <= distW))
			nextMove = EAST;
		else if ( !hasSouthWall(curPosX, curPosY) && (distS <= distE) && (distS <= distN) && (distS <= distW))
			nextMove = SOUTH;
		else if ( !hasWestWall(curPosX, curPosY) && (distW <= distE) && (distW <= distS) && (distW <= distN))
			nextMove = WEST;  

		// 6. Else, go straight if possible
		else if ( orientation == NORTH && !hasNorthWall(curPosX, curPosY) )
			nextMove = NORTH;
		else if ( orientation == EAST && !hasEastWall(curPosX, curPosY) )
			nextMove = EAST;
		else if ( orientation == SOUTH && !hasSouthWall(curPosX, curPosY) )
			nextMove = SOUTH;
		else if ( orientation == WEST && !hasWestWall(curPosX, curPosY) )
			nextMove = WEST;

		// 7. Else, prioritize N > E > S > W
		else if (!hasNorthWall(curPosX, curPosY))
			nextMove = NORTH;
		else if (!hasEastWall(curPosX, curPosY))
			nextMove = EAST;
		else if (!hasSouthWall(curPosX, curPosY))
			nextMove = SOUTH;
		else if (!hasWestWall(curPosX, curPosY))
			nextMove = WEST;
	}

	// Perform next move
	if (nextMove == NORTH && orientation != NORTH) {
		// If has front wall, will align
		if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
		    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
			moveStraight(0.5, searchSpeed, stopSpeed);
			delay_ms(alignTime);
			align();
		}
		else 
			moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, searchSpeed, stopSpeed);
		
		pivotN();
		moveStraight(0.5f + mm_to_counts(pivotOffsetAfter)/cellDistance, searchSpeed, searchSpeed);
		curPosY++;
	}
	else if (nextMove == EAST && orientation != EAST) {
		// If has front wall, will align
		if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
		    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
			moveStraight(0.5, searchSpeed, stopSpeed);
			delay_ms(alignTime);
			align();
		}
		else 
			moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, searchSpeed, stopSpeed);
		
		pivotE();
		moveStraight(0.5f + mm_to_counts(pivotOffsetAfter)/cellDistance, searchSpeed, searchSpeed);
		curPosX++;
	}
	else if (nextMove == SOUTH && orientation != SOUTH) {
		// If has front wall, will align
		if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
		    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
			moveStraight(0.5, searchSpeed, stopSpeed);
			delay_ms(alignTime);
			align();
		}
		else 
			moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, searchSpeed, stopSpeed);
		
		pivotS();
		moveStraight(0.5f + mm_to_counts(pivotOffsetAfter)/cellDistance, searchSpeed, searchSpeed);
		curPosY--;
	}
	else if (nextMove == WEST && orientation != WEST) {
		// If has front wall, will align
		if (useAlignment && ((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
		    (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY)))) {
			moveStraight(0.5, searchSpeed, stopSpeed);
			delay_ms(alignTime);
			align();
		}
		else 
			moveStraight(0.5f + mm_to_counts(pivotOffsetBefore)/cellDistance, searchSpeed, stopSpeed);
		
		pivotW();
		moveStraight(0.5f + mm_to_counts(pivotOffsetAfter)/cellDistance, searchSpeed, searchSpeed);
		curPosX--;
	}
	else {
		moveStraight(1, searchSpeed, searchSpeed);
		if (orientation == NORTH)
			curPosY++;
		else if (orientation == EAST)
			curPosX++;
		else if (orientation == SOUTH)
			curPosY--;
		else if (orientation == WEST)
			curPosX--;
	}
}

void detectWalls(void) {
	bool detectedFrontWall = 0;
	bool detectedLeftWall = 0;
	bool detectedRightWall = 0;
	detectWallOvershoot = (getLeftEncCount() + getRightEncCount())/2;
	
	// Detect front wall
	if (((LFSensorT > frontWallThresholdL) || (RFSensorT > frontWallThresholdR)) && !hasTrace(curPosX, curPosY)) {
		if ((orientation == NORTH && !hasNorthWall(curPosX, curPosY)) || (orientation == EAST && !hasEastWall(curPosX, curPosY)) || (orientation == SOUTH && !hasSouthWall(curPosX, curPosY)) ||
		    (orientation == WEST && !hasWestWall(curPosX, curPosY))) {
			placeWall(curPosX, curPosY, orientation);
			detectedFrontWall = 1;
		}
	}
		
	// Detect left wall
	if (LDSensor > leftWallThreshold && !hasTrace(curPosX, curPosY)) {
		if (orientation == NORTH && !hasWestWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, WEST);
			detectedLeftWall = 1;
		}
		else if (orientation == EAST && !hasNorthWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, NORTH);
			detectedLeftWall = 1;
		}
		else if (orientation == SOUTH && !hasEastWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, EAST);
			detectedLeftWall = 1;
		}
		else if (orientation == WEST && !hasSouthWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, SOUTH);
			detectedLeftWall = 1;
		}
	}
	
	// Detect right wall
	if (RDSensor > rightWallThreshold && !hasTrace(curPosX, curPosY)) {
		if (orientation == NORTH && !hasEastWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, EAST);
			detectedRightWall = 1;
		}
		else if (orientation == EAST && !hasSouthWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, SOUTH);
			detectedRightWall = 1;
		}
		else if (orientation == SOUTH && !hasWestWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, WEST);
			detectedRightWall = 1;
		}
		else if (orientation == WEST && !hasNorthWall(curPosX, curPosY)) {
			placeWall(curPosX, curPosY, NORTH);
			detectedRightWall = 1;
		}
	}
	
	// If a new wall is found, push current cell onto stack
	if ((detectedFrontWall || detectedLeftWall || detectedRightWall) && distance[curPosX][curPosY] != 0 && !hasTrace(curPosX, curPosY)) {
		
		// Check if stack is empty
		if (top_index != 0) {
			targetSpeedX = 0;
			targetSpeedW = 0;
			disableMotorControl();
			setLeftPwm(0);
			setRightPwm(0);
			playLost();
			display("Err detectWalls");
			while(1);
		}
		
		// Check if trapped
		if (!hasPathToTarget(curPosX, curPosY)) {
			moveStraight(0.5f, searchSpeed, stopSpeed);
			targetSpeedX = 0;
			targetSpeedW = 0;
			delay_ms(200);
			disableMotorControl();
			setLeftPwm(0);
			setRightPwm(0);
			display("Trapped");
			playLost();
			printMaze();
			while(1);
		}
		else {
			pushCell(curPosX, curPosY);
		}
			
		if (updateAdjacentCells) {
			// Push cells adjacent to new walls onto stack
			if (detectedFrontWall) {
				pushAdjacentCell(curPosX, curPosY, orientation);
			}
			if (detectedLeftWall) {
				if (orientation == NORTH)
					pushAdjacentCell(curPosX, curPosY, WEST);
				else if (orientation == EAST)
					pushAdjacentCell(curPosX, curPosY, NORTH);
				else if (orientation == SOUTH)
					pushAdjacentCell(curPosX, curPosY, EAST);
				else if (orientation == WEST)
					pushAdjacentCell(curPosX, curPosY, SOUTH);
			}
			if (detectedRightWall) {
				if (orientation == NORTH)
					pushAdjacentCell(curPosX, curPosY, EAST);
				else if (orientation == EAST)
					pushAdjacentCell(curPosX, curPosY, SOUTH);
				else if (orientation == SOUTH)
					pushAdjacentCell(curPosX, curPosY, WEST);
				else if (orientation == WEST)
					pushAdjacentCell(curPosX, curPosY, NORTH);
			}
		}
		
		updateDistances();
	}
	
	placeTrace(curPosX, curPosY);
	detectWallOvershoot = (getLeftEncCount() + getRightEncCount())/2 - detectWallOvershoot;
}


inline void updateDistances(void) {
	struct cellStack poppedCell;
	int32_t minDist;
	
	// While the stack is not empty
	while(top_index > 0) {
				
		// Pop the top of the stack
		poppedCell = popCell();
		
		// Get the min distance of surrounding cells
		minDist = getMinDistance(poppedCell.x, poppedCell.y);
		
		// if the distance of popped cell is not min + 1, update and push neighbors to the stack
		if ((distance[poppedCell.x][poppedCell.y] != minDist + 1) && (distance[poppedCell.x][poppedCell.y] != 0)) {
			// Update popped cell distance
			distance[poppedCell.x][poppedCell.y] = minDist + 1;
			
			// Push its open neighbors to the stack
			if (!hasNorthWall(poppedCell.x, poppedCell.y))
				pushCell(poppedCell.x, poppedCell.y + 1);
			if (!hasEastWall(poppedCell.x, poppedCell.y))
				pushCell(poppedCell.x + 1, poppedCell.y);
			if (!hasSouthWall(poppedCell.x, poppedCell.y))
				pushCell(poppedCell.x, poppedCell.y - 1);
			if (!hasWestWall(poppedCell.x, poppedCell.y))
				pushCell(poppedCell.x - 1, poppedCell.y);
		}
	}
}

void updateDistancesComplete(uint16_t targetX1, uint16_t targetY1, uint16_t targetX2, uint16_t targetY2, uint16_t targetX3, uint16_t targetY3, uint16_t targetX4, uint16_t targetY4) {
	distance[targetX1][targetY1] = 0;
	distance[targetX2][targetY2] = 0;
	distance[targetX3][targetY3] = 0;
	distance[targetX4][targetY4] = 0;
	for (uint32_t i = 0; i < SIZEX*SIZEY; i++) {
		for (uint32_t x = 0; x < SIZEX; x++) {
			for (uint32_t y = 0; y < SIZEY; y++) {
				if ( !((x == targetX1) && (y == targetY1)) && !((x == targetX2) && (y == targetY2)) && !((x == targetX3) && (y == targetY3)) && !((x == targetX4) && (y == targetY4)) ){
					distance[x][y] = getMinDistance(x, y) + 1;
				}
			}
		}
	}
}

inline void pushAdjacentCell(uint16_t x, uint16_t y, uint16_t wall) {
	if (wall == WEST && x > 0 && hasPathToTarget(x-1, y))
		pushCell(x-1, y);
	else if (wall == EAST && x < SIZEX - 1 && hasPathToTarget(x+1, y))
		pushCell(x+1, y);
	else if (wall == SOUTH && y > 0 && hasPathToTarget(x, y-1))
		pushCell(x, y-1);
	else if (wall == NORTH && y < SIZEY - 1 && hasPathToTarget(x, y+1)) 
		pushCell(x, y+1);
}

inline void pushCell(uint16_t x, uint16_t y) {
	if (distance[x][y] != 0) {
		top_index++;
		distStack[top_index].x = x;
		distStack[top_index].y = y;
	}
}

struct cellStack popCell(void) {
	top_index--;
	return distStack[top_index + 1];
}

inline bool hasPathToTarget(uint16_t xPos, uint16_t yPos) {
	// DFS to see if target can be reached from given cell
	bool canReachTarget = 0;
	struct cellStack poppedCell;
	bool visited[SIZEX][SIZEY] = {0};
	
	pushCellVisit(xPos, yPos);
	
	// while the stack is not empty
	while(top_index_visit > 0) {
		// Pop the top of the stack
		poppedCell = popCellVisit();
		
		// Check if target is reached
		if (distance[poppedCell.x][poppedCell.y] == 0) {
			canReachTarget = 1;
		}
		
		// Push its open unvisited neighbors to the stack
		if (!hasNorthWall(poppedCell.x, poppedCell.y) && !visited[poppedCell.x][poppedCell.y+1]) {
			pushCellVisit(poppedCell.x, poppedCell.y + 1);
			visited[poppedCell.x][poppedCell.y+1] = 1;
		}
		if (!hasEastWall(poppedCell.x, poppedCell.y) && !visited[poppedCell.x+1][poppedCell.y]) {
			pushCellVisit(poppedCell.x + 1, poppedCell.y);
			visited[poppedCell.x+1][poppedCell.y] = 1;
		}
		if (!hasSouthWall(poppedCell.x, poppedCell.y) && !visited[poppedCell.x][poppedCell.y-1]) {
			pushCellVisit(poppedCell.x, poppedCell.y - 1);
			visited[poppedCell.x][poppedCell.y-1] = 1;
		}
		if (!hasWestWall(poppedCell.x, poppedCell.y) && !visited[poppedCell.x-1][poppedCell.y]) {
			pushCellVisit(poppedCell.x - 1, poppedCell.y);
			visited[poppedCell.x-1][poppedCell.y] = 1;
		}
		
		visited[poppedCell.x][poppedCell.y] = 1;

	}
	return canReachTarget;
}

inline void pushCellVisit(uint16_t x, uint16_t y) {
	top_index_visit++;
	visitStack[top_index_visit].x = x;
	visitStack[top_index_visit].y = y;
}

struct cellStack popCellVisit(void) {
	top_index_visit--;
	return visitStack[top_index_visit + 1];
}
