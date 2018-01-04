#include "main.h"

// System controlled
bool useMotorControl1 = 0;
bool useMotorControl2 = 0;
bool useIRSensors = 0;
bool useAngleFeedback = 0;
bool useOnlyGyroFeedback = 0;
bool useOnlyEncFeedback = 0;

// User controlled
bool useAlignment;
int32_t speedRunProfile;
int32_t searchProfile;
bool useFrontWallCalibration;
bool useCrashDetection;
bool useAngularCorrection;

float leftEncCount = 0;
float rightEncCount = 0;
float encCount = 0;
float oldLeftEncCount = 0;
float oldRightEncCount = 0;
float leftEncChange = 0;
float rightEncChange = 0;
float encChange = 0;	// current speed in counts/ms
float distanceLeft = 0;

float targetSpeedX = 0;	// speed in counts/ms
float targetSpeedW = 0;	// speed in counts/ms

float curSpeedX = 0;
float curSpeedW = 0;
float encFeedbackX = 0;
float encFeedbackW = 0;
float gyroFeedback = 0;
float errorX = 0;
float errorW = 0;
float oldErrorX = 0;
float oldErrorW = 0;
int32_t posPwmX = 0;
int32_t posPwmW = 0;
int32_t leftPwmTotal;
int32_t rightPwmTotal;

// Speed settings
int32_t maxPwm;
int32_t maxFanPwm;
int32_t maxBoostPwm;
float stopSpeed;
float searchSpeed;
float turnSpeed;
float traceSpeed;
float straightSpeed;
float diagSpeed;
float alignSpeed;
float calibrateSpeed;
float calibrateAccX;
float calibrateDecX;

// Constant variables
float cellDistance;	// counts
float wheelDistance;	// mm
float wheelDistanceL;	// during left turns
float wheelDistanceR;	// during right turns
int32_t sensorScale;
float gyroScale1;
float gyroScale2;
float gyroScale3;
float angleScale;
float centerToBackDist;
float frontEdgeDist;
float frontEdgeDistTurn;
float pivotOffsetBefore;
float pivotOffsetAfter;
int32_t alignSensorScale;

float accX;	// m/s/s
float decX;
float accW;	// deg/s/s
float decW;
float alignAccX;
float alignDecX;
float alignAccW;
float alignDecW;
float alignKdW;
float alignKpW;
float straightAccX;
float straightDecX;
float straightAccW;
float straightDecW;
float straightKdX;
float straightKpX;
float straightKpW;
float straightKdW;
float diagAccX;
float diagDecX;
float diagAccW;
float diagDecW;
float diagKdX;
float diagKpX;
float diagKdW;
float diagKpW;

int32_t pwmLimitCounter;
int32_t pwmLimitTime;
int32_t crashWallThreshold;
int32_t pivotTimeLimit;

// Motor encoder PID
float kpX;
float kdX;
float kpW;
float kdW;

// updateSpeed2 variables
float setSpeedX = 0;
float setSpeedW = 0;
float setSpeedL = 0;
float setSpeedR = 0;
float errorL = 0;
float errorR = 0;
float errorCumL = 0;
float errorCumR = 0;
float prevErrorL = 0;
float prevErrorR = 0;
float posPwmL = 0;
float posPwmR = 0;

int32_t hugTime;
int32_t hugPwm;
float hugSensorScale;

void setLeftPwm(int32_t pwm) {
	
	if (pwm > maxPwm)
		pwm = maxPwm;
	else if (pwm < -maxPwm)
		pwm = -maxPwm;
	
	if (USESLOWDECAY) {
		// Slow decay
		if (pwm >= 0) {
			LPWMA = maxPwm;
			LPWMB	= maxPwm - pwm; 
		}
		else	{
			pwm = -pwm;
			LPWMA = maxPwm - pwm;
			LPWMB	= maxPwm; 
		}
	}
	else {
		// Fast decay
		if(pwm >= 0)	{
			LPWMA = pwm;
			LPWMB	= 0; 
		}          
		else	{   
			pwm = -pwm;
			LPWMA = 0;
			LPWMB	= pwm; 
		}
	}
}

void setRightPwm(int32_t pwm) {
	if (pwm > maxPwm)
		pwm = maxPwm;
	else if (pwm < -maxPwm)
		pwm = -maxPwm;

	if (USESLOWDECAY) {
		// Slow decay
		if (pwm >= 0) {
			RPWMA = maxPwm;
			RPWMB = maxPwm - pwm;
		}
		else {
			pwm = -pwm;
			RPWMA = maxPwm - pwm;
			RPWMB = maxPwm;
		}
	}
	else {
		// Fast decay
		if(pwm >= 0)	{
			RPWMA = pwm;
			RPWMB = 0; 
		}
		else	{
			pwm = -pwm;
			RPWMA = 0;
			RPWMB = pwm; 
		}
	}
}

void setLeftBoostPwm(int32_t pwm) {
	if (pwm < 0)
		pwm = 0;
	if(pwm > maxBoostPwm)
		pwm = maxBoostPwm;

	LEFTBOOSTPWM = pwm;
}

void setRightBoostPwm(int32_t pwm) {
	if (pwm < 0)
		pwm = 0;
	if(pwm > maxBoostPwm)
		pwm = maxBoostPwm;

	RIGHTBOOSTPWM = pwm;
}

void setFanPwm(int32_t pwm) {
	if (pwm < 0)
		pwm = 0;
	if(pwm > maxFanPwm)
		pwm = maxFanPwm;

	FANPWM = pwm;
}


// Update motor speed to match target speed
void updateSpeed1(void) {

	// Get data from encoders and update distance left to travel
	leftEncCount = getLeftEncCount();
	rightEncCount = getRightEncCount();
	
	leftEncChange = leftEncCount - oldLeftEncCount;
	rightEncChange = rightEncCount - oldRightEncCount;
	encChange = (leftEncChange + rightEncChange)/2;
	
	oldLeftEncCount = leftEncCount;
	oldRightEncCount = rightEncCount;
	
	leftEncCount += leftEncChange;
	rightEncCount += rightEncChange;
	encCount = (leftEncCount + rightEncCount)/2;
	
	// Update current	speed
	if (curSpeedX < targetSpeedX) {
		curSpeedX += mm_to_counts(accX)/1000;
		if (curSpeedX > targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	else if (curSpeedX > targetSpeedX) {
		curSpeedX -= mm_to_counts(decX)/1000;
		if (curSpeedX < targetSpeedX)
			curSpeedX = targetSpeedX;
	}
	
	if (curSpeedW < targetSpeedW) {
		curSpeedW +=  deg_to_counts(accW)/1000/1000;
		if (curSpeedW > targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	else if (curSpeedW > targetSpeedW) {
		curSpeedW -= deg_to_counts(decW)/1000/1000;
		if (curSpeedX < targetSpeedW)
			curSpeedW = targetSpeedW;
	}
	
	/*
	// Subtract distance left by encoder count with least change
	if (targetSpeedW == 0 && targetSpeedX > 0) {
		distanceLeft -= (leftEncChange < rightEncChange)? leftEncChange:rightEncChange;
	}
	else {
		distanceLeft -= encChange;
	}
	*/
	
	distanceLeft -= encChange;
	
	// PD control 
	float rotationalFeedback;
	
	encFeedbackX = (rightEncChange + leftEncChange)/2;
	encFeedbackW = (rightEncChange - leftEncChange)/2;
	
	/*
	// Gyro feedback calculations
	gyroFeedback = 0;
	if (USEGYRO1 && USEGYRO2)
		gyroFeedback = ((-aSpeed1/gyroScale1) + (-aSpeed2/gyroScale2))/2;
	else if (USEGYRO1)
		gyroFeedback = -aSpeed1/gyroScale1;
	else if (USEGYRO2)
		gyroFeedback = -aSpeed2/gyroScale2;
	
	// Override gyro1 and gyro2 at fast angular velocities
	if (USEGYRO3 && (aSpeed1 > ASPEED1CUTOFF || aSpeed2 > ASPEED2CUTOFF || aSpeed1 < -ASPEED1CUTOFF || aSpeed2 < -ASPEED2CUTOFF)) {
		gyroFeedback = -aSpeed3/gyroScale3;
		LED1_ON;
	}
	else {
		LED1_OFF;
	}
	*/
	gyroFeedback = -aSpeed3/gyroScale3;
	
	if (useOnlyGyroFeedback)
		rotationalFeedback = gyroFeedback;
	else if (useOnlyEncFeedback)
		rotationalFeedback = encFeedbackW;
	else 
		rotationalFeedback = gyroFeedback + encFeedbackW;
	
	// Angle feedback
	if (useAngleFeedback && useAngularCorrection) {
		rotationalFeedback -= angleT/angleScale;
	}
	
	// IR Sensor feedback
	if (useIRSensors)
		rotationalFeedback -= getSensorError()/sensorScale;
	
	errorX += curSpeedX - encFeedbackX;
	errorW += curSpeedW - rotationalFeedback;
	
	posPwmX = (kpX * errorX + kdX * (errorX - oldErrorX));
	posPwmW = (kpW * errorW + kdW * (errorW - oldErrorW));
	
	oldErrorX = errorX;
	oldErrorW = errorW;
	
	leftPwmTotal = posPwmX - posPwmW;
	rightPwmTotal = posPwmX + posPwmW;
	
	// Decrease right PWM if left saturates
	if (leftPwmTotal > maxPwm)
		rightPwmTotal -= leftPwmTotal - maxPwm;
	else if (leftPwmTotal < -maxPwm)
		rightPwmTotal += maxPwm - leftPwmTotal;
	
	// Decrease left PWM if right saturates
	if (rightPwmTotal > maxPwm)
		leftPwmTotal -= rightPwmTotal - maxPwm;
	else if (rightPwmTotal < -maxPwm) 
		leftPwmTotal += maxPwm - rightPwmTotal;
	
	setLeftPwm(leftPwmTotal);
	setRightPwm(rightPwmTotal);
}

void updateSpeed2(void)
{
	static float kp = 10;
	static float kd = 5;
	static float ki = 0;
	accX = 1000000;
	decX = 1000000;
	accW = 1000000;
	decW = 1000000;

	// Get data from encoders and update distance left to travel
	leftEncCount = getLeftEncCount();
	rightEncCount = getRightEncCount();
	
	leftEncChange = leftEncCount - oldLeftEncCount;
	rightEncChange = rightEncCount - oldRightEncCount;
	encChange = leftEncChange + rightEncChange;
	
	oldLeftEncCount = leftEncCount;
	oldRightEncCount = rightEncCount;
	
	leftEncCount += leftEncChange;
	rightEncCount += rightEncChange;
	distanceLeft -= encChange;
	
	if (setSpeedX < targetSpeedX) {
		setSpeedX += mm_to_counts(accX)/1000;
		if (setSpeedX > targetSpeedX)
			setSpeedX = targetSpeedX;
	}
	else if (setSpeedX > targetSpeedX) {
		setSpeedX -= mm_to_counts(decX)/1000;
		if (setSpeedX < targetSpeedX)
			setSpeedX = targetSpeedX;
	}
	if (setSpeedW < targetSpeedW) {
		setSpeedW += deg_to_counts(accW)/1000/1000;
		if (setSpeedW > targetSpeedW)
			setSpeedW = targetSpeedW;
	}
	else if (setSpeedW > targetSpeedW) {
		setSpeedW -= deg_to_counts(decW)/1000/1000;
		if (setSpeedW < targetSpeedW)
			setSpeedW = targetSpeedW;
	}
	
	// calculate left error
	setSpeedL = setSpeedX - setSpeedW;
	errorL += setSpeedL - leftEncChange;
	posPwmL = kp*errorL + kd*(errorL-prevErrorL) + ki*errorCumL;
	
	// calculate right error
	setSpeedR = setSpeedX + setSpeedW;
	errorR += setSpeedR - rightEncChange;
	posPwmR = kp*errorR + kd*(errorR-prevErrorR) + ki*errorCumR;
	
	// update integral input
	errorCumL += errorL;
	errorCumR += errorR;
	
	// update derivative input
	prevErrorL = errorL;
	prevErrorR = errorR;
	
	setLeftPwm(posPwmL);
	setRightPwm(posPwmR);
}

// Get deceleration needed given distance left to travel, current speed, and final speed
float getDecNeeded(float d, float Vi, float Vf) {
	float decNeeded;
	if (d == 0) {
		d = 1;
	}
	decNeeded = (Vf*Vf - Vi*Vi)/d/2;
	return (decNeeded < 0)? -decNeeded: decNeeded;	// Always returns non-negative
}

// convert counts to mm
float counts_to_mm(float counts) {
	return (counts/(cellDistance/180));
}


// convert mm to counts
float mm_to_counts(float mm) {
	return (mm*(cellDistance/180));
}

// convert counts to deg
float counts_to_deg(float counts) {
	return (counts/(3.14159f*wheelDistance*cellDistance/180/360));
}

// convert deg to counts
float deg_to_counts(float deg) {
	return (deg*(3.14159f*wheelDistance*cellDistance/180/360));
}

void enableMotorControl(void) {
	resetMotorParameters();
	useMotorControl1 = 1;
}

void disableMotorControl(void) {
	useMotorControl1 = 0;
	useMotorControl2 = 0;
	resetMotorParameters();
}

void resetMotorParameters(void) {

	errorL = 0;
	errorR = 0;
	errorCumL = 0;
	errorCumR = 0;
	prevErrorL = 0;
	prevErrorR = 0;
	posPwmL = 0;
	posPwmR = 0;
	
	leftEncCount = 0;
	rightEncCount = 0;
	encCount = 0;
	oldLeftEncCount = 0;
	oldRightEncCount = 0;
	leftEncChange = 0;
	rightEncChange = 0;
	encChange = 0;
	distanceLeft = 0;

	targetSpeedX = 0;
	targetSpeedW = 0;

	curSpeedX = 0;
	curSpeedW = 0;
	encFeedbackX = 0;
	encFeedbackW = 0;
	gyroFeedback = 0;
	angle1 = 0;
	angle2 = 0;
	angle3 = 0;
	angleT = 0;
	errorX = 0;
	errorW = 0;
	oldErrorX = 0;
	oldErrorW = 0;
	posPwmX = 0;
	posPwmW = 0;
	
	setLeftPwm(0);
	setRightPwm(0);
	
	resetLeftEncCount();
	resetRightEncCount();
}


/**
 * Straight movement
 */
void moveStraight(float cells, float maxSpeed, float endSpeed) {
	accX = straightAccX;
	decX = straightDecX;
	accW = straightAccW;
	decW = straightDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;

	int32_t timeOutCounter = 5000;	// ms
	int32_t curt = millis();
	pwmLimitCounter = 0;
	angle1 = angle2 = angleT = angle3 = 0;
	distanceLeft = cells*cellDistance - counts_to_mm(detectWallOvershoot);
	// Forward movement
	useAngleFeedback = 1;
	float curDistLeft = distanceLeft; // non-volatile distance left
	if (cells > 0 && curDistLeft > 0) {
		useIRSensors = 1;
		targetSpeedX = mm_to_counts(maxSpeed);
		curDistLeft = distanceLeft;

		while(curDistLeft > 0) {
			// check timeout
			if (millis() - curt > timeOutCounter)
				break;
			/*
			if ((cells > 0.9f) && (curDistLeft - mm_to_counts(90) > 0)) {
				if (getDecNeeded(curDistLeft - mm_to_counts(135), curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(decX)/1000)
					accX = 0;
			}
			*/
			
			if (getDecNeeded(curDistLeft, curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(decX)/1000) {
				targetSpeedX = mm_to_counts(endSpeed);
			}
			else {
				targetSpeedX = mm_to_counts(maxSpeed);
			}
			
			// Crash detection
			if (useCrashDetection && (curDistLeft > mm_to_counts(90)) && ((LFSensorT > LFSensorLookup[crashWallThreshold]) || (RFSensorT > RFSensorLookup[crashWallThreshold]))) {
				targetSpeedX = 0;
				targetSpeedW = 0;
				disableMotorControl();
				setLeftPwm(0);
				setRightPwm(0);
				
				display("Err moveStraight");
				playLost();
				printMaze();
				while(1);
			}
			
			// Overshoot detection
			if (useFrontWallCalibration && LFSensorT > LFSensorLookup[(int32_t)frontEdgeDist] && RFSensorT > RFSensorLookup[(int32_t)frontEdgeDist]) {
				decX = 10;
				//delay_ms(alignTime);
				break;
			}
			
			curDistLeft = distanceLeft;
		}
	}
	
	// Backward movement
	else if (cells < 0 && distanceLeft < 0) {
		// swap acc/dec when reverse
		accX = straightDecX;
		decX = straightAccX;
		targetSpeedX = -mm_to_counts(maxSpeed);
		curDistLeft = distanceLeft;
		while(curDistLeft < 0) {
			// check timeout
			if (millis() - curt > timeOutCounter)
				break;
			
			if (getDecNeeded(curDistLeft, curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(accX)/1000) { // swap acc/dec when reverse
				targetSpeedX = -mm_to_counts(endSpeed);
			}
			else {
				targetSpeedX = -mm_to_counts(maxSpeed);
			}
			
			curDistLeft = distanceLeft;
		}
		targetSpeedX = mm_to_counts(endSpeed);
	}
	targetSpeedX = mm_to_counts(endSpeed);
	distanceLeft = 0;
	useAngleFeedback = 0;
	useIRSensors = 0;
	
	// Reset detect wall overshoot
	detectWallOvershoot = 0;
}

/**
 * Forward movement without acc/dec
 */
void moveStraightTurn(float cells, float maxSpeed, float endSpeed) {
	accX = straightAccX;
	decX = straightDecX;
	accW = straightAccW;
	decW = straightDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	int32_t timeOutCounter = 5000;	// ms
	int32_t curt = millis();
	
	pwmLimitCounter = 0;
	angle1 = angle2 = angleT = angle3 = 0;
	distanceLeft = cells*cellDistance;
	useAngleFeedback = 1;
	// Forward movement only
	float curDistLeft = distanceLeft; // non-volatile distance left
	if (cells > 0 && distanceLeft > 0) {
		useIRSensors = 1;
		targetSpeedX = mm_to_counts(maxSpeed);
		curDistLeft = distanceLeft;
		while(curDistLeft > 0) {
			// check timeout
			if (millis() - curt > timeOutCounter)
				break;
			
			if (getDecNeeded(curDistLeft, curSpeedX, mm_to_counts(endSpeed)) > mm_to_counts(decX)/1000) {
				targetSpeedX = mm_to_counts(endSpeed);
			}
			else {
				targetSpeedX = mm_to_counts(maxSpeed);
			}
			
			curDistLeft = distanceLeft;
		}
	}
	
	targetSpeedX = mm_to_counts(endSpeed);
	distanceLeft = 0;
	useAngleFeedback = 0;
	useIRSensors = 0;
}

/**
 * Calibrate before turn
 */
void calibrateTurn(float maxSpeed, float endSpeed) {
	accX = straightAccX;
	decX = straightDecX;
	accW = straightAccW;
	decW = straightDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	useIRSensors = 1;
	
	targetSpeedX = mm_to_counts(endSpeed);

	// If overshoot, compensate distance up to 10mm
	while((LFSensorT < LFSensorLookup[80 + (int32_t)frontEdgeDistTurn]) && (RFSensorT < RFSensorLookup[80 + (int32_t)frontEdgeDistTurn]));
	
	targetSpeedX = mm_to_counts(endSpeed);

	useIRSensors = 0;
}

/**
 * Straight movement along diagonals
 */
void moveDiagonal(float count, float maxSpeed, float endSpeed) {
	accX = diagAccX;
	decX = diagDecX;
	accW = diagAccW;
	decW = diagDecW;
	kpX = diagKpX;
	kdX = diagKdX;
	kpW = diagKpW;
	kdW = diagKdW;
	
	pwmLimitCounter = 0;
	angle1 = angle2 = angleT = angle3 = 0;
	distanceLeft = count*cellDistance*0.7071f;
	// Forward movement
	useIRSensors = 1;
	useAngleFeedback = 1;
	while(distanceLeft > 0 && count > 0) {
		if (getDecNeeded(distanceLeft, curSpeedX, mm_to_counts(endSpeed)) < mm_to_counts(decX)/1000) {
			targetSpeedX = mm_to_counts(maxSpeed); 
		}
		else {
			targetSpeedX = mm_to_counts(endSpeed);
		}
	}
	targetSpeedX = mm_to_counts(endSpeed);
	
	// Backward movement
	useIRSensors = 0;
	while(distanceLeft < 0 && count < 0) {
		if (getDecNeeded(distanceLeft, curSpeedX, mm_to_counts(endSpeed)) < mm_to_counts(decX)/1000) {
			targetSpeedX = -mm_to_counts(maxSpeed);
		}
		else {
			targetSpeedX = mm_to_counts(endSpeed);
		}
	}
	targetSpeedX = mm_to_counts(endSpeed);
	useAngleFeedback = 0;
}


/**
 *  Angular movement for curve turns
 *  +speedW turns right, -speedW turns left
 *  time is in ms, speedX is in m/s, speedW is in deg/s, accW is in deg/s/s
 *  Allow 10mm for compensation
 */
void turn(struct turnProfile profile) {
	pwmLimitCounter = 0;
	
	// change wheelDistance for left and right turns
	if (profile.speedW > 0)	// right turn
		wheelDistance = wheelDistanceR;
	else if (profile.speedW < 0)
		wheelDistance = wheelDistanceL;
	
	// If overshoot over 10mm, no frontWall calibration
	if ((counts_to_mm(detectWallOvershoot) > 10) && ((profile.d1-counts_to_mm(detectWallOvershoot)) > 0)) {
		moveStraightTurn(mm_to_counts(profile.d1-counts_to_mm(detectWallOvershoot))/cellDistance, profile.endSpeed, profile.endSpeed);
	}
	// Otherwise if has front wall, compensate distance before turn
	else if (((orientation == NORTH && hasNorthWall(curPosX, curPosY)) || (orientation == EAST && hasEastWall(curPosX, curPosY)) ||
	     (orientation == SOUTH && hasSouthWall(curPosX, curPosY)) || (orientation == WEST && hasWestWall(curPosX, curPosY))) &&
	      useFrontWallCalibration && profile.d1-counts_to_mm(detectWallOvershoot) > 0) {
		calibrateTurn(profile.speedX, profile.endSpeed);
		if (profile.d1 - 10 - counts_to_mm(detectWallOvershoot) > 0)
			moveStraightTurn(mm_to_counts(profile.d1-10-counts_to_mm(detectWallOvershoot))/cellDistance, profile.endSpeed, profile.endSpeed);
	}
	// Otherwise, no front wall compensation
	else if ((profile.d1 - counts_to_mm(detectWallOvershoot)) > 0){
		moveStraightTurn(mm_to_counts(profile.d1-counts_to_mm(detectWallOvershoot))/cellDistance, profile.endSpeed, profile.endSpeed);
	}
	
	accW = profile.accW;
	decW = profile.decW;
	
	useOnlyGyroFeedback = 1;
	//useOnlyEncFeedback = 1;
	
	int32_t curt = millis();
	int32_t angleStart = angle3;
	
	// T1
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = deg_to_counts(-profile.speedW)/1000;
	elapse_ms(profile.t1, curt);
	// T2
	kpW = profile.kpW2;
	kdW = profile.kdW2;
	elapse_ms(profile.t1 + profile.t2, curt);
	// T3
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = stopSpeed;
	elapse_ms(profile.t1 + profile.t2 + profile.t3, curt);
	
	// Crash detection
	if (useCrashDetection && (profile.speedW > 0) && (angle3 - angleStart < 0)) {
		targetSpeedX = 0;
		targetSpeedW = 0;
		disableMotorControl();
		setLeftPwm(0);
		setRightPwm(0);
		
		display("Err turn");
		playLost();
		while(1);
	}
	else if (useCrashDetection && (profile.speedW < 0) && (angle3 - angleStart > 0)) {
		targetSpeedX = 0;
		targetSpeedW = 0;
		disableMotorControl();
		setLeftPwm(0);
		setRightPwm(0);
		
		display("Err turn");
		playLost();
		while(1);
	}
	
	useOnlyGyroFeedback = 0;
	useOnlyEncFeedback = 0;
	
	moveStraightTurn(mm_to_counts(profile.d2)/cellDistance, profile.endSpeed, profile.endSpeed);
	
	// Reset detect wall overshoot
	detectWallOvershoot = 0;
}

/**
 *  Angular movement for diagonal turns
 *  +speedW turns right, -speedW turns left
 *  time is in ms, speedX is in m/s, speedW is in deg/s, accW is in deg/s/s
 */
void turnDiag(struct turnProfile profile) {
	pwmLimitCounter = 0;
	
	// change wheelDistance for left and right turns
	if (profile.speedW > 0)	// right turn
		wheelDistance = wheelDistanceR;
	else if (profile.speedW < 0)
		wheelDistance = wheelDistanceL;
	
	moveStraight(mm_to_counts(profile.d1)/cellDistance, profile.speedX, profile.speedX);
	
	accW = profile.accW;
	decW = profile.decW;
	
	useOnlyGyroFeedback = 1;
	
	int32_t curt = millis();
	
	// T1
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = deg_to_counts(-profile.speedW)/1000;
	elapse_ms(profile.t1, curt);
	// T2
	kpW = profile.kpW2;
	kdW = profile.kdW2;
	elapse_ms(profile.t1 + profile.t2, curt);
	// T3
	kpW = profile.kpW13;
	kdW = profile.kdW13;
	targetSpeedW = stopSpeed;
	elapse_ms(profile.t1 + profile.t2 + profile.t3, curt);
	
	useOnlyGyroFeedback = 0;
	
	moveStraight(mm_to_counts(profile.d2)/cellDistance, profile.speedX, profile.endSpeed);
}

/**
 *  Angular movement for pivot turns
 *  +speedW turns right, -speedW turns left
 *  time is in ms, speedX is in m/s, speedW is in deg/s, accW is in deg/s/s
 */
void pivotTurn(struct pivotTurnProfile profile) {
	resetMotorParameters();
	pwmLimitCounter = 0;
	useMotorControl2 = 1;
	
	delay_ms(profile.delay);
	
	int32_t curt;
	int32_t startAngle = angle3;
	
	// pivot left
	if (profile.speedW < 0) {
		curt = millis();
		while (angle3 - startAngle > profile.angle) {
			targetSpeedW = deg_to_counts(-profile.speedW)/1000;
			
			// Crash detection
			if (useCrashDetection && (millis() - curt > pivotTimeLimit)) {
				targetSpeedX = 0;
				targetSpeedW = 0;
				disableMotorControl();
				setLeftPwm(0);
				setRightPwm(0);
				
				display("Err pivotTurn");
				playLost();
				while(1);
			}
		}
		targetSpeedW = 0;
	}
	
	// pivot right
	else if (profile.speedW > 0) {
		curt = millis();
		while (angle3 - startAngle < profile.angle) {
			targetSpeedW = deg_to_counts(-profile.speedW)/1000;

			// Crash detection
			if (useCrashDetection && (millis() - curt > pivotTimeLimit)) {
				targetSpeedX = 0;
				targetSpeedW = 0;
				disableMotorControl();
				setLeftPwm(0);
				setRightPwm(0);
				
				display("Err pivotTurn");
				playLost();
				while(1);
			}
		}
		targetSpeedW = 0;
	}
	delay_ms(profile.delay);
	useMotorControl2 = 0;
	resetMotorParameters();
}

void align(void){
	int32_t stuckTime = 1000;	// timer to prevent getting stuck
	int32_t tempSensorScale = sensorScale;
	accX = alignAccX;
	decX = alignDecX;
	accW = alignAccW;
	decW = alignDecW;
	kpX = straightKpX;
	kdX = straightKdX;
	kdW = alignKdW;
	kpW = alignKpW;
	sensorScale = alignSensorScale;
	int32_t curDistLeft;
	useIRSensors = 1;
	
	// Prevent getting stuck
	int32_t curt;
	
	// Perform alignment
	for (int32_t i = 0; i < 1; i++) {
		
		// Too close, move back
		if (sensorLookup[(int32_t)frontEdgeDist - alignErrorTolerance] < (LFSensorT + RFSensorT)/2) {
			curt = millis();
			
			// swap acc/dec when reverse
			accX = alignDecX;
			decX = alignAccX;
			
			curDistLeft = 0;
			targetSpeedX = -mm_to_counts(alignSpeed);
			while(sensorLookup[(int32_t)frontEdgeDist] < (LFSensorT + RFSensorT)/2) {
				// Check if stuck
				if (millis() - curt > stuckTime) {
					break;
				}
				
				// get dist from front wall
				int32_t i;
				for(i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
					if (sensorLookup[i] <= (LFSensorT + RFSensorT)/2) {
						break;
					}
				}
				
				// distance left should be negative
				curDistLeft = mm_to_counts(i) - mm_to_counts(frontEdgeDist);
				if (curDistLeft > 0)
					curDistLeft = 0;
				
				// if sensor reading is greater than all stored values, move backward
				if (i == SENSOR_DATA_ARRAY_SIZE)
					targetSpeedX = -mm_to_counts(alignSpeed);
				else if (getDecNeeded(curDistLeft, curSpeedX, stopSpeed) < mm_to_counts(alignAccX)/1000) { // swap acc/dec when reverse
					targetSpeedX = -mm_to_counts(alignSpeed);
				}
				else {
					targetSpeedX = stopSpeed;
				}
			}
		}
		
		// Too far, move forward
		else if (sensorLookup[(int32_t)frontEdgeDist + alignErrorTolerance] > (LFSensorT + RFSensorT)/2) {
			curt = millis();
			
			curDistLeft = 0;
			targetSpeedX = mm_to_counts(alignSpeed);
			while(sensorLookup[(int32_t)frontEdgeDist] > (LFSensorT + RFSensorT)/2) {
				// Check if stuck
				if (millis() - curt > stuckTime)
					break;
				
				int32_t i;
				for(i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
					if (sensorLookup[i] <= (LFSensorT + RFSensorT)/2) {
						break;
					}
				}
				curDistLeft = mm_to_counts(i - (int32_t)frontEdgeDist);
				if (curDistLeft < 0)
					curDistLeft = 0;
				
				// if sensor reading is greater than all stored values
				if (i == SENSOR_DATA_ARRAY_SIZE)
					targetSpeedX = mm_to_counts(alignSpeed);
				else if (getDecNeeded(curDistLeft, curSpeedX, stopSpeed) < mm_to_counts(alignDecX)/1000) {
					targetSpeedX = mm_to_counts(alignSpeed);
				}
				else {
					targetSpeedX = stopSpeed;
				}
			}
		}
	}
	targetSpeedX = stopSpeed;
	delay_ms(alignTime);	// prevent overshoot
	sensorScale = tempSensorScale;
	useIRSensors = 0;
}


/* Hug Front Wall */
void hugFrontWall(int32_t LSensorVal, int32_t RSensorVal) {
	int32_t tempPwm = maxPwm;
	int curt = millis();
	maxPwm = hugPwm;
	while(millis() - curt < hugTime) {
		setLeftPwm(hugSensorScale*(LSensorVal - LFSensorT));
		setRightPwm(hugSensorScale*(RSensorVal - RFSensorT));
	}
	maxPwm = tempPwm;
}
