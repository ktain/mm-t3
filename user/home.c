#include "main.h"

int32_t leftCount = 0;
int32_t rightCount = 0;
int32_t option = 0;
int32_t signalThreshold;
bool busy = 0;

int32_t startX = 0;
int32_t startY = 0;

int32_t destX1 = 7;
int32_t destY1 = 7;

int32_t destX2 = 7;
int32_t destY2 = 8;

int32_t destX3 = 8;
int32_t destY3 = 7;

int32_t destX4 = 8;
int32_t destY4 = 8;

char* options[] = {
	"Sea1", // 0
	"Sea2",
	"Sea3",
	"SeaP",
	"Run1",
	"Run2",
	"Run3",
	"Dia1",	// 7
	"Dia2",
	"Smo1",
	"Save",
	"Load",
	"IR Cal", // 12
	"Gyro Cal",
	"Rst",
	"Random Search",
	"No adjacent update",
	"No align",
	"No front wall cal",
	"No angle feedback"
};


bool waitForSignal(void) {
	display("Waiting...");
	while(RFSensorT < signalThreshold) {
		if (!busy) {
			clearDisplay();
			return 0;
		}
	}
	playChirp();
	clearDisplay();
	return 1;
}

// Option refresh tracker
void refreshOptions(void) {
	bool needToRefresh = 0;
	if (getLeftEncCount() - leftCount > 2000) {
		option++;
		if (option > NUMOPTIONS - 1) {
			option = 0;
		}
		beep(50, 2000);
		leftCount = getLeftEncCount();
		rightCount = getRightEncCount();
		needToRefresh = 1;
	}
	else if (getLeftEncCount() - leftCount < -2000) {
		option--;
		if (option < 0) {
			option = NUMOPTIONS - 1;
		}
		beep(50, 2000);
		leftCount = getLeftEncCount();
		rightCount = getRightEncCount();
		needToRefresh = 1;
	}
	
	if (needToRefresh) {
		display(options[option]);
	}

}


void back(void) {
	busy = 0;
}

void enter(void) {
	busy = 1;
	if (!waitForSignal()) {
		return;
	}
	
	delay_ms(500);
	int32_t tempCrashDetection;
	
	switch(option) {
		case 0: 
			calibrateGyrosTemp();
		
			searchProfile = 0; 
			searchSpeed = 0.5;	// m/s
			turnSpeed = 0.5;	// m/s
			traceSpeed = 1.5;	// Backtracking
			straightAccX = 6.0;	// m/s/s
			straightDecX = 2.5;	// m/s/s
		
			playMario();
			if (floodSearch(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4) == 1) {	// if success (5, 5, 5, 4, 4, 4, 4, 5)
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 0; 
				searchSpeed = 0.5;	// m/s
				turnSpeed = 0.5;	// m/s
				traceSpeed = 1.5;	// Backtracking
				straightAccX = 6.0;	// m/s/s
				straightDecX = 2.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 1: 
			calibrateGyrosTemp();
		
			searchProfile = 1; 
			searchSpeed = 0.7;	// m/s
			turnSpeed = 0.7;	// m/s
			traceSpeed = 2.0;	// Backtracking
			straightAccX = 6.0;	// m/s/s
			straightDecX = 3.0;	// m/s/s
			
			playMario();
			if (floodSearch(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 6.0;	// m/s/s
				straightDecX = 3.0;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 2: 
			calibrateGyrosTemp();
		
			searchProfile = 2; 
			searchSpeed = 0.8;	// m/s
			turnSpeed = 0.8;	// m/s
			traceSpeed = 2.0;	// Backtracking
			straightAccX = 8.0;	// m/s/s
			straightDecX = 4.5;	// m/s/s
			
			playMario();
			if (floodSearch(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 6.0;	// m/s/s
				straightDecX = 3.0;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 3: // Pivot search
			calibrateGyrosTemp();
		
			searchProfile = 0; 
			searchSpeed = 0.7;	// m/s
			traceSpeed = 1.5;	// Backtracking
			straightAccX = 6.0;	// m/s/s
			straightDecX = 4.0;	// m/s/s
		
			playMario();
			tempCrashDetection = useCrashDetection;
			useCrashDetection = 0;
			if (floodSearchPivot(destX1, destY1, destX2, destY2, destX3, destY3, destX4, destY4) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 0;
				searchSpeed = 0.7;	// m/s
				traceSpeed = 1.5;	// Backtracking
				straightAccX = 6.0;	// m/s/s
				straightDecX = 4.0;	// m/s/s
				if (floodSearchPivot(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			useCrashDetection = tempCrashDetection;
			break;
		case 4: //speedRunProfile = 0; 
			calibrateGyrosTemp();
		
			searchProfile = 1;
			turnSpeed = 0.7;	// m/s
			straightSpeed = 2.5;	// Speed run
			straightAccX = 10.0;	// m/s/s
			straightDecX = 5.0;	// m/s/s
		
			if (speedRunClassic(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 5: //speedRunProfile = 1; 
			calibrateGyrosTemp();
	
			searchProfile = 2;
			turnSpeed = 0.8;	// m/s
			straightSpeed = 2.8;	// Speed run
			straightAccX = 12.0;	// m/s/s
			straightDecX = 6.0;	// m/s/s
		
			if (speedRunClassic(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 6: //speedRunProfile = 2; 
			calibrateGyrosTemp();
		
			searchProfile = 2;
			turnSpeed = 0.8;	// m/s
			straightSpeed = 2.9;	// Speed run
			straightAccX = 13.0;	// m/s/s
			straightDecX = 7.0;	// m/s/s
		
			if (speedRunClassic(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 7: 
			calibrateGyrosTemp();
		
			speedRunProfile = 0; 
			searchProfile = 0;
			diagSpeed = 1.0;	// m/s
			straightSpeed = 2.8;	// Speed run
			straightAccX = 12.0;	// m/s/s
			straightDecX = 6.0;	// m/s/s
			diagAccX = 6.0;
			diagDecX = 3.0;
			
			if (speedRunDiag(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 8:	
			calibrateGyrosTemp();
		
			speedRunProfile = 1; 
			searchProfile = 1;
			turnSpeed = 0.7;
			diagSpeed = 1.5;	// m/s
			straightSpeed = 2.8;	// Speed run
			straightAccX = 12.0;	// m/s/s
			straightDecX = 6.0;	// m/s/s
			diagAccX = 6.0;
			diagDecX = 3.0;
			
			if (speedRunDiag(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 9:	// Smooth 1
			calibrateGyrosTemp();
		
			speedRunProfile = 0; 
			searchProfile = 1;
			searchSpeed = 0.7;	// m/s
			turnSpeed = 0.7;	// m/s
			diagSpeed = 1.0;	// m/s
			straightSpeed = 2.8;	// Speed run
			straightAccX = 12.0;	// m/s/s
			straightDecX = 6.0;	// m/s/s
			diagAccX = 6.0;
			diagDecX = 3.0;
		
			if (speedRunSmooth(destinationX, destinationY) == 1) {	// if success
				playVictory();
				delay_ms(200);
				
				calibrateGyrosTemp();
				
				searchProfile = 1; 
				searchSpeed = 0.7;	// m/s
				turnSpeed = 0.7;	// m/s
				traceSpeed = 2.0;	// Backtracking
				straightAccX = 8.0;	// m/s/s
				straightDecX = 4.5;	// m/s/s
				if (floodSearch(startX, startY, startX, startY, startX, startY, startX, startY) == 0)
					playLost();
			}
			else {
				playLost();
			}
			break;
		case 10:saveMaze();
			break;
		case 11:loadMaze();
			break;
		case 12:calibrateIRSensors();
			break;
		case 13:calibrateGyros();
			break;
		case 14:NVIC_SystemReset();
			break;
		case 15: // random search
			calibrateGyrosTemp();
		
			useCrashDetection = 0;
			
			searchProfile = 1; 
			searchSpeed = 0.7;	// m/s
			straightSpeed = 0.7;	// Speed run
			turnSpeed = 0.7;	// m/s
			traceSpeed = 1.5;	// Backtracking
			straightAccX = 6.0;	// m/s/s
			straightDecX = 3.0;	// m/s/s
		
			playMario();
			randomMovement();
		
			//printMaze();
			curPosX = 0;
			curPosY = 0;
			orientation = NORTH;
			break;
		case 16: // don't update adjacent cells
			updateAdjacentCells = 0;
			break;
		case 17: // don't use alignment
			useAlignment = 0;
			break;
		case 18: // don't use front wall calibration
			useFrontWallCalibration = 0;
			break;
		case 19: // dont use angular correction
			useAngularCorrection = 0;
			break;
		default:
			break;
	}
	display(options[option]);
	playPrelude();
	busy = 0;
}
