#include "main.h"


/* Move at random */
void randomMovement(void) {
	curPosX = 0;
	curPosY = 0;
	orientation = NORTH;
	placeTrace(curPosX, curPosY);
	
	updateDistancesComplete(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4);
	
	enableMotorControl();
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5f, turnSpeed, turnSpeed);
	curPosY++;
	int32_t choice;
	
	while(1) {
		choice = micros()%3;
		
		// place trace
		placeTrace(curPosX, curPosY);
		
		// if found target, close untraced cells
		if (distance[curPosX][curPosY] == 0) {
			moveStraight(0.5, turnSpeed, stopSpeed);
			delay_ms(200);
			pivotLeft180();
			
			if (orientation == NORTH) {
				orientation = SOUTH;
			}
			else if (orientation == EAST) {
				orientation = WEST;
			}
			else if (orientation == SOUTH) {
				orientation = NORTH;
			}
			else if (orientation == WEST) {
				orientation = EAST;
			}
			
			closeUntracedCells();
			updateDistancesComplete(startX, startY, startX, startY, startX, startY, startX, startY);
			
			// remove traces
			for (int32_t i = 0; i < SIZEX; i++) {
				for (int32_t j = 0; j < SIZEY; j++) {
					cell[i][j] &= 0xffff - TRACEBIT;
				}
			}
			break;
		}
		
		// move forward
		if (choice == 0 && LFSensorT < frontWallThresholdL || RFSensorT < frontWallThresholdR) {
			if (orientation == NORTH && !(curPosX == 0 && curPosY+1 == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosY++;
			}
			else if (orientation == EAST && !(curPosX+1 == 0 && curPosY == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosX++;
			}
			else if (orientation == SOUTH && !(curPosX == 0 && curPosY-1 == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosY--;
			}
			else if (orientation == WEST && !(curPosX-1 == 0 && curPosY == 0)) {
				moveStraight(1, turnSpeed, turnSpeed);
				curPosX--;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		// curve left
		else if (choice == 1 && LDSensor < leftWallThreshold) {
			if (orientation == NORTH && !(curPosX-1 == 0 && curPosY == 0)) {
				curveLeft90();
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == EAST && !(curPosX == 0 && curPosY+1 == 0)) {
				curveLeft90();
				curPosY++;
				orientation = NORTH;
			}
			else if (orientation == SOUTH && !(curPosX+1 == 0 && curPosY == 0)) {
				curveLeft90();
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == WEST && !(curPosX == 0 && curPosY-1 == 0)) {
				curveLeft90();
				curPosY--;
				orientation = SOUTH;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		// curve right
		else if (choice == 2 && RDSensor < rightWallThreshold) {
			if (orientation == NORTH && !(curPosX+1 == 0 && curPosY == 0)) {
				curveRight90();
				curPosX++;
				orientation = EAST;
			}
			else if (orientation == EAST && !(curPosX == 0 && curPosY-1 == 0)) {
				curveRight90();
				curPosY--;
				orientation = SOUTH;
			}
			else if (orientation == SOUTH && !(curPosX-1 == 0 && curPosY == 0)) {
				curveRight90();
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == WEST && !(curPosX == 0 && curPosY+1 == 0)) {
				curveRight90();
				curPosY++;
				orientation = NORTH;
			}
			else {
				moveBack();
				if (orientation == NORTH) {
					curPosY--;
					orientation = SOUTH;
				}
				else if (orientation == EAST) {
					curPosX--;
					orientation = WEST;
				}
				else if (orientation == SOUTH) {
					curPosY++;
					orientation = NORTH;
				}
				else if (orientation == WEST) {
					curPosX++;
					orientation = EAST;
				}
			}
		}
		else if ((LFSensorT > frontWallThresholdL || RFSensorT > frontWallThresholdR) && LDSensor > leftWallThreshold && RDSensor > rightWallThreshold) {
			// deadend
			moveBack();
			if (orientation == NORTH) {
				curPosY--;
				orientation = SOUTH;
			}
			else if (orientation == EAST) {
				curPosX--;
				orientation = WEST;
			}
			else if (orientation == SOUTH) {
				curPosY++;
				orientation = NORTH;
			}
			else if (orientation == WEST) {
				curPosX++;
				orientation = EAST;
			}
		}
		
	}
	disableMotorControl();
}

/*
void randomMovement(void) {
	
	enableMotorControl();
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + .5f, turnSpeed, turnSpeed);
	int32_t choice;
	
	while(1) {
		choice = micros()%3;
		
		// move forward
		if (choice == 0 && LFSensorT < frontWallThresholdL && RFSensorT < frontWallThresholdR)
			moveStraight(1, turnSpeed, turnSpeed);

		// curve left
		else if (choice == 1 && LDSensor < leftWallThreshold)
			curveLeft90();
		// curve right
		else if (choice == 2 && RDSensor < rightWallThreshold)
			curveRight90();
		
		else if (LFSensorT > frontWallThresholdL && RFSensorT > frontWallThresholdR && LDSensor > leftWallThreshold && RDSensor > rightWallThreshold) {
			// deadend
			moveBack();
		}
		
	}
	disableMotorControl();
}
*/
