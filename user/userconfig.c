#include "main.h"

void loadSettings(void) {

	/* Default settings */
	
	// Location
	curPosX = 0;	// 0 is bottom left, horizontal
	curPosY = 0;	// 0 is bottom left, vertical
	orientation = NORTH;
	
	// Search settings
	useBacktracking = 1;
	useAlignment = 1;
	useFrontWallCalibration = 1;
	updateAdjacentCells = 1;
	useAngularCorrection = 1;
	
	// Gyro settings
	gyroScale1 = 58.15;			// Higher == larger angle
	gyroScale2 = 58.15;			// 6mv/deg/s
	gyroScale3 = 9.7;	// 9.70		// 0.67mv/deg/s, higher == larger angle
	angleScale = 5000;			// lower == more feedback
	outZOffset = -1.0;	//1.5		// (LY3200) positive == turns more to the left
	rateOffset = 0.0;			// (ADXRS620) positive == turns more to the left
	
	// Fan settings
	maxFanPwm = 500;
	
	// Default speed settings
	maxPwm = 999;		// max motor pwm, 12.45v supply
	maxBoostPwm = 400;	// max boost motor pwm, 8.4v supply
	stopSpeed = 0;		// m/s
	searchSpeed = 0.8;	// m/s
	turnSpeed = 0.8;	// m/s
	traceSpeed = 2.0;	// Backtracking
	straightSpeed = 3.0;	// Speed run
	diagSpeed = 1.0;
	calibrateSpeed = 0.2;	// IR Sensor calibration
	calibrateAccX = 0.1;
	calibrateDecX = 0.1;
	straightAccX = 6.0;	// m/s/s
	straightDecX = 3.0;	// m/s/s
	straightAccW = 1000;	// deg/s/s, used for straights
	straightDecW = 1000;  	// deg/s/s, used for straights
	straightKpX = 2;	// used for straights
	straightKdX = 5;
	straightKpW = 2.0;
	straightKdW = 20;
	diagAccX = 3;		// used for diagonals
	diagDecX = 3;
	diagAccW = 5000;
	diagDecW = 5000;
	diagKpX = 2;
	diagKdX = 5;
	diagKpW = 2.0;
	diagKdW = 20;
	speedRunProfile = 0;
	searchProfile = 0;
	pivotTurnDelay = 20;	// ms

	// Load IR sensor and gyro settings from flash
	loadSensorData();
	loadGyroData();
	
	// IR Sensor constants
	LDMidVal = LDSensorLookup;			// 1360
	RDMidVal = RDSensorLookup;			// 1050
	leftWallThreshold = LDMidVal/3;			// when mouse is at half a cell distance
	rightWallThreshold = RDMidVal/3;		// 250
	frontWallThresholdL = LFSensorLookup[200];
	frontWallThresholdR = RFSensorLookup[200];
	approachThresholdL = LFSensorLookup[100];	// Threshold for angular correction during straights
	approachThresholdR = RFSensorLookup[100];
	signalThreshold = sensorLookup[70];		// Detect finger
	sensorScale = 100;				// lower == more feedback
	useSensorLow = 1;
	useSensorMedium = 1;
	useSensorHigh = 0;
	
	
	// Alignment
	alignErrorTolerance = 1;// +- mm
	alignSpeed = 0.1;	// m/s
	alignTime = 100;	// for angular correction
	alignSensorScale = 100;	// lower == more feedback
	alignAccX = 0.6;	// m/s/s
	alignDecX = 0.6;	// m/s/s
	alignAccW = 100;	// deg/s/s
	alignDecW = 100;	// deg/s/s
	alignKpW = 0.3;
	alignKdW = 50;
	hugPwm = 200;
	hugTime = 300;		// ms
	hugSensorScale = 0.5;
	
	
	// Measurements
	cellDistance = 25870;		// 25658 counts
	wheelDistance = 67.5; 		// 68.08 mm for 0.5m/s turn, larger == faster turn angle
	wheelDistanceR = 67.5;		// during right turns
	wheelDistanceL = 67.5;		// during left turns
	centerToBackDist = 45;		// center of rotation from back edge (curve turns)
	frontEdgeDistTurn = 29;		// 84-(mouseLength-centerToBackDist)
	frontEdgeDist = 29;		// Pivot
	pivotOffsetBefore = 0;		// y-axis offset before pivoting 90 deg
	pivotOffsetAfter = 0;		// y-axis offset after pivoting 90 deg
	
	// Crash detection settings
	useCrashDetection = 1;
	crashWallThreshold = 30;	// mm from front wall
	aSpeed3Threshold = 1300;
	pwmLimitTime = 500;		// ms
	pivotTimeLimit = 3000;		// ms
	
	// Print settings
	showTrace = 1;

}
