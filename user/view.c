#include "main.h"

bool showTrace;

/* 
 * Function name: printMaze()
 * Description: Prints the maze
 */
void printMaze(void) {
	for (int32_t y = SIZEY - 1; y >= 0; y--) {

		/*
		* Print north wall, mouse, and traces
		*/
		for (int32_t x = 0; x < SIZEX; x++) {
			if ( hasNorthWall(x, y) )
				if (HIDEEAST)
					printf("+----");
				else 
					printf("+----+");
				else
				if (HIDEEAST)
					printf("+    ");
				else
					printf("+    +");
		}

		if (HIDEEAST) {
			printf("+");
		}

		printf("\n\r");



		/*
		* Print west and east wall, mouse, and traces
		*/
		for (int32_t x = 0; x < SIZEX; x++) {

			// Print west wall
			if ( hasWestWall(x, y) )
				printf("|");
			else
				printf(" ");

			// Print if mouse present
			if ( x == curPosX && y == curPosY ) {
				if (orientation == NORTH)
					printf("  ^ ");
				else if (orientation == EAST)
					printf("  > ");
				else if (orientation == SOUTH)
					printf("  v ");
				else if (orientation == WEST)
					printf("  < ");
			}

			// Print markers
			else if (hasTrace(x, y) && showTrace) {
				printf("  . ");
			} else {
				printf("%4d", distance[x][y]);
			}

			// Opt to print east wall
			if (!HIDEEAST) {
				if ( hasEastWall(x, y) )
					printf("|");
				else
					printf(" ");
			}
		}

		// Print east boundary if necessary
		if (HIDEEAST){
			printf("|");
		}

		printf("\n\r");

		// Opt to print south wall
		if (!HIDESOUTH) {
			for (int32_t x = 0; x < SIZEX; x++) {
				if ( hasSouthWall(x, y) ) {
					if (HIDEEAST) 
						printf("+----");
					else
						printf("+----+");
				}
				else {
					if (HIDEEAST)
						printf("+    ");
					else 
						printf("+    +");
				}
			}

			if (HIDEEAST) {
				printf("+");
			}

			printf("\n\r");
		}
	}

	// Print south boundary if necessary 
	if(HIDESOUTH) {
		for(int32_t x = 0; x < SIZEX; x++) {
			if(HIDEEAST)
			  printf("+----");
			else 
			  printf("+----+");     
		}

		if (HIDEEAST) {
			printf("+");
		}

		printf("\n\r");
	}
}

void plotSensorData(void) {
	/*
	printf("scaledRate1 = %f scaledRate2 = %f scaledOutz = %f\r\n", rate1, rate2, Outz);
	printf("{XYPLOT|SET|TITLE=Sensor Distances}");
	for (int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE ; i++) {
		printf("{XYPLOT|DATA|Left|%d|%d}", i, LFSensorLookup[i]);
		printf("{XYPLOT|DATA|Right|%d|%d}", i, RFSensorLookup[i]);
	}
	printf("\n\r");
	*/
}

void transmit(void) {
	/*
	while(1) {
		printf("{TABLE|SET|Voltage|%d}", voltage);
		printf("{TABLE|SET|LENC|%d}", getLeftEncCount());
		printf("{TABLE|SET|RENC|%d}", getRightEncCount());
		printf("{TABLE|SET|angle1|%f}", angle1);
		printf("{TABLE|SET|angle2|%f}", angle2);
		printf("{TABLE|SET|angle3|%f}", angle3);
		printf("{TABLE|SET|aSpeed1|%f}", aSpeed1);
		printf("{TABLE|SET|aSpeed2|%f}", aSpeed2);
		printf("{TABLE|SET|aSpeed3|%f}", aSpeed3);
		printf("{TABLE|SET|temp1|%d}", read_Temp1);
		printf("{TABLE|SET|temp2|%d}", read_Temp2);
		printf("{TABLE|SET|LFT|%d}", LFSensorT);
		printf("{TABLE|SET|RFT|%d}", RFSensorT);
		printf("{TABLE|SET|LD|%d}", LDSensor);
		printf("{TABLE|SET|RD|%d}", RDSensor);
	
		printf("{TIMEPLOT:Battery|D|Voltage|T|%d}", voltage);
		printf("{TIMEPLOT:Rate|D|Rate1|T|%d}", read_Rate1);
		printf("{TIMEPLOT:Rate|D|Rate2|T|%d}", read_Rate2);
		printf("TIMEPLOT:Angle|D|Angle1|T|%f}", angle1);
		printf("TIMEPLOT:Angle|D|Angle2|T|%f}", angle2);
		printf("TIMEPLOT:Angle3|D|Angle3|T|%f}", angle3);
		printf("TIMEPLOT:Angle|D|AngleAvg|T|%f}", (angle1 + angle2)/2);
		printf("TIMEPLOT:Speed|D|curSpeedX|T|%f}", curSpeedX);
		printf("{TIMEPLOT:IRSensor|D|LFTotal|T|%d}", LFSensorT);
		printf("{TIMEPLOT:IRSensor|D|RFTotal|T|%d}", RFSensorT);
	}
	*/
}
