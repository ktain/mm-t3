#include "main.h"

/* Sensor lookup tables */
int32_t sensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t LFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t RFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
int32_t LDSensorLookup;
int32_t RDSensorLookup;

/* Sensor usage settings */
bool useSensorLow;
bool useSensorMedium;
bool useSensorHigh;

/* Sensor constants */
int32_t LDMidVal;
int32_t RDMidVal;
int32_t leftWallThreshold;	// when mouse is at half a cell distance
int32_t rightWallThreshold;
int32_t frontWallThresholdL;
int32_t frontWallThresholdR;
int32_t approachThresholdL;
int32_t approachThresholdR;
int32_t alignTime;
int32_t alignErrorTolerance;	// +- mm

/* Gyro constants */
int32_t scaledRate1;
int32_t scaledRate2;
int32_t scaledOutz;
float outZOffset;
float rateOffset;

/* Sensor values updated every ms */
volatile int32_t voltage;
volatile int32_t LFSensorL;
volatile int32_t LFSensorM;
volatile int32_t LFSensorH;
volatile int32_t RFSensorL;
volatile int32_t RFSensorM;
volatile int32_t RFSensorH;
volatile int32_t LDSensor;
volatile int32_t RDSensor;
volatile int32_t LFSensorT;
volatile int32_t RFSensorT;

int32_t aSpeed1;
int32_t aSpeed2;
int32_t aSpeed3;
int32_t angle1; 
int32_t angle2;
int32_t angle3;
int32_t angleT;
int32_t Outz;
int32_t aSpeed3Threshold;


/* Read IR Sensors */
void readIRSensors(void)
{
	
	//Read DC value
	LFSensorL = LFSensorM = LFSensorH = read_LF_Sensor;
	//Read LF Sensor
	LF_EM_ON1;
	delay_us(EMITTER_ON_TIME);
	LFSensorL = read_LF_Sensor - LFSensorL;

	LF_EM_ON2;
	delay_us(EMITTER_ON_TIME);
	LFSensorH = read_LF_Sensor - LFSensorH;
	
	LF_EM_OFF1;
	delay_us(EMITTER_ON_TIME);
	LFSensorM = read_LF_Sensor - LFSensorM;
	LF_EM_OFF2;

	if(LFSensorL < 0)	//error check
		LFSensorL = 0;
	if(LFSensorM < 0)
		LFSensorM = 0;
	if(LFSensorH < 0)
		LFSensorH = 0;
	delay_us(EMITTER_OFF_TIME);
	
	// Read DC value
	RFSensorL = RFSensorM = RFSensorH = read_RF_Sensor;	
	//Read RF Sensor
	RF_EM_ON1;
	delay_us(EMITTER_ON_TIME);
	RFSensorL = read_RF_Sensor - RFSensorL;

	RF_EM_ON2;
	delay_us(EMITTER_ON_TIME);
	RFSensorH = read_RF_Sensor - RFSensorH;
	
	RF_EM_OFF1;
	delay_us(EMITTER_ON_TIME);
	RFSensorM = read_RF_Sensor - RFSensorM;
	RF_EM_OFF2;
	
	if(RFSensorL < 0)
		RFSensorL = 0;
	if(RFSensorM < 0)
		RFSensorM = 0;
	if(RFSensorH < 0)
		RFSensorH = 0;
 	delay_us(EMITTER_OFF_TIME);
	
	
	// Read DC value
	LDSensor = read_LD_Sensor;
	// Read LD Sensor
	LD_EM_ON;
	delay_us(EMITTER_ON_TIME);
	LDSensor = read_LD_Sensor - LDSensor;
	LD_EM_OFF;
	if(LDSensor < 0)
		LDSensor = 0;
	delay_us(EMITTER_OFF_TIME);
	
	// Read DC value
	RDSensor = read_RD_Sensor;	
	// Read RD Sensor
	RD_EM_ON;
	delay_us(EMITTER_ON_TIME);
	RDSensor = read_RD_Sensor - RDSensor;
	RD_EM_OFF;
	if(RDSensor < 0)
		RDSensor = 0;
	
	// Calculator front sensor totals
	int32_t LMHCount = 0;
	LFSensorT = 0;
	RFSensorT = 0;
	if (useSensorLow) {
		LFSensorT += LFSensorL;
		RFSensorT += RFSensorL;
		LMHCount++;
	}
	if (useSensorMedium) {
		LFSensorT += LFSensorM;
		RFSensorT += RFSensorM;
		LMHCount++;
	}
	if (useSensorHigh) {
		LFSensorT += LFSensorH;
		RFSensorT += RFSensorH;
		LMHCount++;
	}
	if (LMHCount > 0) {
		LFSensorT /= LMHCount;
		RFSensorT /= LMHCount;
	}
}

// Returns sensor error 
int32_t getSensorError(void) {
	int32_t sensorError = 0;

	// Use front wall
	if (LFSensorT > approachThresholdL || RFSensorT > approachThresholdR) {
		angle1 = angle2 = angle3 = angleT = 0;
		for(int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
			if (LFSensorLookup[i] < LFSensorT  || RFSensorLookup[i] < RFSensorT) {
				sensorError = ((LFSensorT - RFSensorT) - (LFSensorLookup[i] - RFSensorLookup[i]));
				break;
			}
		}
	}
	// Otherwise use side walls
	else if (LDSensor > LDMidVal)
		sensorError = LDMidVal - LDSensor;
	else if (RDSensor > RDMidVal)
		sensorError = RDSensor - RDMidVal;
	
	// Positive sensor error turns to the left
	return sensorError;
}

/* Read gyro */
void readGyro(void) {
	
	int32_t i;
	int32_t sampleNum = GYRO_SAMPLE_NUM;
	aSpeed1 = 0;
	aSpeed2 = 0;
	aSpeed3 = 0;
	for(i = 0; i < sampleNum; i++) {
		if (USEGYRO1)
			aSpeed1 += read_Rate1;	// Clockwise rotation produces positive output
		if (USEGYRO2)
			aSpeed2 += read_Rate2;
		if (USEGYRO3)
			aSpeed3 += read_Outz;
		
		delay_us(GYRO_SAMPLE_INTERVAL);
	}
	
	if (USEGYRO1) {
		aSpeed1 *= GYRO_MULTIPLIER/sampleNum;
		aSpeed1 -= scaledRate1;	// Rate1 * 10000, if angle goes up, increase (hotter = drifts up)
		aSpeed1 /= GYRO_MULTIPLIER;
		angle1 += aSpeed1;
	}
	
	if (USEGYRO2) {
		aSpeed2 *= GYRO_MULTIPLIER/sampleNum;
		aSpeed2 -= scaledRate2;	// Rate2 * 10000, if angle goes up, increase (hotter = drifts up)
		aSpeed2 /= GYRO_MULTIPLIER;
		angle2 += aSpeed2;
	}
	
	if (USEGYRO3) {
		aSpeed3 *= GYRO_MULTIPLIER/sampleNum;
		aSpeed3 -= scaledOutz;	// Outz * 10000, if angle goes up, increase
		aSpeed3 /= GYRO_MULTIPLIER;
		aSpeed3 = -aSpeed3;	// flip +/-
		angle3 += aSpeed3;
	}
	
	if (USEGYRO1 && USEGYRO2)
		angleT = (angle1 + angle2)/2;
	else if (USEGYRO1)
		angleT = angle1;
	else if (USEGYRO2)
		angleT = angle2;
}


/* Read voltage */
void readVoltage(void) {
	voltage = read_Voltage;
}


/* Calibrate front sensors */
void calibrateIRSensors(void) {
	delay_ms(CALIBRATE_IR_SENSOR_START_DELAY);
	
	accX = calibrateDecX;
	decX = calibrateAccX;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;

	enableMotorControl();
	targetSpeedX = -mm_to_counts(calibrateSpeed);

	// Record raw values every mm
	for (int32_t i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		LFSensorLookup[i] = LFSensorT;
		RFSensorLookup[i] = RFSensorT;
		while(-counts_to_mm((getLeftEncCount() + getRightEncCount())/2) < i) {
			if (i == 90) {
				LDSensorLookup = LDSensor + LDSensor/20;
				RDSensorLookup = RDSensor + RDSensor/20;
			}
		}
	}
	
	targetSpeedX = stopSpeed;
	
	// Single pass averaging of 2 most similar neighbors
	for (int32_t i = 2; i < SENSOR_DATA_ARRAY_SIZE-2; i++) {
		int32_t a, b, c, d, e, temp;
		
		// Left sensor
		a = LFSensorLookup[i-2];
		b = LFSensorLookup[i-1];
		c = LFSensorLookup[i];
		d = LFSensorLookup[i+1];
		e = LFSensorLookup[i+2];
		
		// Move lowest value to a
		if (b<a) {temp = a; a = b; b = temp;}
		if (c<a) {temp = a; a = c; c = temp;}
		if (d<a) {temp = a; a = d; d = temp;}
		if (e<a) {temp = a; a = e; e = temp;}
		
		// Move largest value to e
		if (b>e) {temp = e; e = b; b = temp;}
		if (c>e) {temp = e; e = c; c = temp;}
		if (d>e) {temp = e; e = d; d = temp;}
		
		// Average b, c, d
		LFSensorLookup[i] = (b + c + d)/3;
		
		// Repeat for right sensor
		a = RFSensorLookup[i-2];
		b = RFSensorLookup[i-1];
		c = RFSensorLookup[i];
		d = RFSensorLookup[i+1];
		e = RFSensorLookup[i+2];
		
		// Move lowest value to a
		if (b<a) {temp = a; a = b; b = temp;}
		if (c<a) {temp = a; a = c; c = temp;}
		if (d<a) {temp = a; a = d; d = temp;}
		if (e<a) {temp = a; a = e; e = temp;}
		
		// Move largest value to e
		if (b>e) {temp = e; e = b; b = temp;}
		if (c>e) {temp = e; e = c; c = temp;}
		if (d>e) {temp = e; e = d; d = temp;}
		
		// Average b, c, d
		RFSensorLookup[i] = (b + c + d)/3;
	}
	
	// Extrapolate first and last couple elements
	LFSensorLookup[0] = LFSensorLookup[1] = LFSensorLookup[2];
	RFSensorLookup[0] = RFSensorLookup[1] = RFSensorLookup[2];
	
	// Ensure values are monotonically decreasing
	for (int32_t i = 1; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		if (LFSensorLookup[i] > LFSensorLookup[i-1]) 
			LFSensorLookup[i] = LFSensorLookup[i-1];
		if(RFSensorLookup[i] > RFSensorLookup[i-1])
			RFSensorLookup[i] = RFSensorLookup[i-1];
	}
	
	// Store average of left and right
	for (int32_t index = 0; index < SENSOR_DATA_ARRAY_SIZE; index++) {
		sensorLookup[index] = (LFSensorLookup[index] + RFSensorLookup[index])/2;
	}
	
	delay_ms(500);
	disableMotorControl();
	
	// Save sensor data to flash
	saveSensorData();

	// Load sensor data from flash
	loadSensorData();
	
}


/* Calibrate gyros */
void calibrateGyros(void) {
	accX = calibrateDecX;
	decX = calibrateAccX;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	useOnlyEncFeedback = 1;
	enableMotorControl();
	delay_ms(CALIBRATE_GYRO_START_DELAY);

	float rate1 = 0;
	float rate2 = 0;
	float Outz = 0;
	int32_t sampleNum = CALIBRATE_GYRO_SAMPLE_NUM;

	for (int32_t i = 0; i < sampleNum; i++) {
		rate1 += read_Rate1 - rateOffset;
		rate2 += read_Rate2 - rateOffset;
		Outz += read_Outz + outZOffset;
		delay_ms(CALIBRATE_GYRO_SAMPLE_DELAY);
	}
	scaledRate1 = rate1*GYRO_MULTIPLIER/sampleNum;
	scaledRate2 = rate2*GYRO_MULTIPLIER/sampleNum;
	scaledOutz = Outz*GYRO_MULTIPLIER/sampleNum;
	
	disableMotorControl();
	useOnlyEncFeedback = 0;

	// Save sensor data to flash
	saveGyroData();

	// Load sensor data from flash
	loadGyroData();
}


/* Calibrate gyros temporarily without saving */
void calibrateGyrosTemp(void) {
	accX = calibrateDecX;
	decX = calibrateAccX;
	kpX = straightKpX;
	kdX = straightKdX;
	kpW = straightKpW;
	kdW = straightKdW;
	
	useOnlyEncFeedback = 1;
	enableMotorControl();
	delay_ms(CALIBRATE_GYRO_START_DELAY);

	float rate1 = 0;
	float rate2 = 0;
	float Outz = 0;
	int32_t sampleNum = CALIBRATE_GYRO_SAMPLE_NUM;

	for (int32_t i = 0; i < sampleNum; i++) {
		rate1 += read_Rate1 - rateOffset;
		rate2 += read_Rate2 - rateOffset;
		Outz += read_Outz + outZOffset;
		delay_ms(CALIBRATE_GYRO_SAMPLE_DELAY);
	}
	scaledRate1 = rate1*GYRO_MULTIPLIER/sampleNum;
	scaledRate2 = rate2*GYRO_MULTIPLIER/sampleNum;
	scaledOutz = Outz*GYRO_MULTIPLIER/sampleNum;
	
	disableMotorControl();
	useOnlyEncFeedback = 0;
}

