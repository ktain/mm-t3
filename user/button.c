#include "main.h"

void button0(void) {
	playChirp();
	delay_ms(100);
	busy = 1;
	
	waitForSignal();
	delay_ms(1000);
	
	enableMotorControl();
	/*
	speedRunProfile = 1; 
	searchProfile = 1;
	diagSpeed = 1.5;	// m/s
	turnSpeed = 0.7;
	straightSpeed = 2.8;	// Speed run
	straightAccX = 8.0;	// m/s/s
	straightDecX = 6.0;	// m/s/s
	diagAccX = 6.0;
	diagDecX = 3.0;
	
	for (int i = 0; i < 8; i++) {
		pivotRight90();
		delay_ms(200);
	}
	for (int i = 0; i < 8; i++) {
		pivotLeft90();
		delay_ms(200);
	}
	*/
	
	
	/*
	useAngularCorrection = 0;
	useFrontWallCalibration = 0;
	
	searchProfile = 2;
	searchSpeed = 0.8;	// m/s
	turnSpeed = 0.8;	// m/s
	straightAccX = 8.0;	// m/s/s
	straightDecX = 4.5;	// m/s/s
	
			
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 1.5f, turnSpeed, turnSpeed);
	curveRight90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveRight90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveRight90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveRight90();
	moveStraight(1, turnSpeed, 0.0);
	delay_ms(1000);
	
	disableMotorControl();
	enableMotorControl();
	
	waitForSignal();
	delay_ms(1000);
	
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 1.5f, turnSpeed, turnSpeed);
	curveLeft90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveLeft90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveLeft90();
	moveStraight(1, turnSpeed, turnSpeed);
	curveLeft90();
	moveStraight(1, turnSpeed, 0.0);
	delay_ms(1000);
	*/
	
	/*
	for (int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++)
		printf("%dmm %d\r\n", i, RFSensorLookup[i]);
	*/
	
	/*
	while(1) {
		displayVoltage();
		delay_ms(100);
	}
	*/

	/*
	straightSpeed = 0.5;
	straightAccX = 8;	// m/s/s
	straightDecX = 4.0;	// m/s/s
	moveStraight(2, straightSpeed, 0);
	delay_ms(500);
	*/
	
	/*
	while(1) {
		printf("enc %d\r\n", (getLeftEncCount() + getRightEncCount())/2);
		delay_ms(100);
	}
	*/
	
	/*
	while(1) {
		printf("LD %d RD %d\r\n", LDSensor, RDSensor);
		delay_ms(100);
	}
	*/
	
	/*
	moveStraight(0.2, 0.8, 0);
	delay_ms(100);
	pivotRight180();
	moveStraight(0.2, 0.8, 0);
	delay_ms(100);
	pivotRight180();
	moveStraight(0.2, 0.8, 0);
	delay_ms(100);
	pivotRight180();
	moveStraight(0.2, 0.8, 0);
	delay_ms(100);
	pivotRight180();
	delay_ms(500);
	*/
	
	/*
	accX = straightAccX;
	decX = straightDecX;
	kpX = straightKpX;
	kdX = straightKdX;
	
	enableMotorControl();
	
	targetSpeedX = mm_to_counts(1);
	delay_ms(1000);
	*/
	
	/*
	moveStraight(5, 1, 0);
	delay_ms(1000);
	printf("lenc %d renc %d", getLeftEncCount(), getRightEncCount());
	*/
	
	/*
	useOnlyEncFeedback = 1;
	accX = 1;
	decX = 0.1;
	accW = 100000;
	decW = 100000;	// d/s/s
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	targetSpeedX = mm_to_counts(0);
	targetSpeedW = deg_to_counts(72)/1000;
	delay_ms(10000);
	targetSpeedX = 0;
	targetSpeedW = 0;
	*/
	
	/*
	enableMotorControl2();
	for (int i = 0; i < 4; i++) {
		int32_t a3 = angle3;
		while (angle3 - a3 > -74300) {	// 74300 left	// 73400 right	// 300 deg/s
			targetSpeedW = deg_to_counts(300)/1000;
		}
		targetSpeedW = 0;
		delay_ms(500);
	}
	*/
	
	/*
	int32_t a3 = angle3;
	waitForSignal();
	printf("a3 %d\r\n", angle3 - a3); // -90 -74500
	*/
	
	
	/*
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5 , 0.5, 0.5);
	while(1) {
		moveStraight(1, 0.5, 0.5);
		curveLeft90();
	}
	delay_ms(1000);
	*/
	
	/*
	searchProfile = 2;
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5f, 0.85, 0.85);
	curveRight90();
	moveStraight(0.5, 0.85, 0);
	delay_ms(500);
	//align();
	delay_ms(1000);
	*/
	
	/*
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance, 0.7, 0.7);
	curveRight45SD();
	moveStraight(mm_to_counts(127.28)/cellDistance, 0.7, 0.7);
	curveLeft45DS();
	decX = 1000;
	targetSpeedX = 0;
	delay_ms(500);
	*/
	
	/*
	moveStraight(mm_to_counts(127.28)/cellDistance, turnSpeed, turnSpeed);
	curveLeft45DS();
	decX = 1000;
	targetSpeedX = 0;
	delay_ms(500);
	*/
	
	/*
	searchProfile = 1;
	calibrateGyros();
	for (int i = 0; i < 12; i++) {
		pivotRight90();
		delay_ms(500);
	}
	for (int i = 0; i < 12; i++) {
		pivotLeft90();
		delay_ms(500);
	}
	*/
	speedRunProfile = 1; 
	searchProfile = 2;
	diagSpeed = 1.5;	// m/s
	turnSpeed = 0.8;
	straightSpeed = 2.8;	// Speed run
	straightAccX = 8.0;	// m/s/s
	straightDecX = 6.0;	// m/s/s
	diagAccX = 6.0;
	diagDecX = 3.0;
	
	/*
	moveStraight(mm_to_counts(84 - centerToBackDist)/cellDistance + 0.5f, 0.8, 0.8);
	curveLeft90();
	moveStraight(1 , 0.8, 0);
	delay_ms(200);
	*/
	moveStraight(13, 0.8, 0);
	delay_ms(200);
	
	disableMotorControl();
	
	//back();
	busy = 0;
}


void button1(void) {
	playChirp();
	delay_ms(100);
	
	enter();
}
