#ifndef SENSOR
#define SENSOR

extern int32_t sensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t LFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t RFSensorLookup[SENSOR_DATA_ARRAY_SIZE];
extern int32_t LDSensorLookup;
extern int32_t RDSensorLookup;

extern bool useSensorLow;
extern bool useSensorMedium;
extern bool useSensorHigh;

extern volatile int32_t voltage;
extern volatile int32_t LFSensorL;
extern volatile int32_t LFSensorM;
extern volatile int32_t LFSensorH;
extern volatile int32_t RFSensorL;
extern volatile int32_t RFSensorM;
extern volatile int32_t RFSensorH;
extern volatile int32_t LDSensor;
extern volatile int32_t RDSensor;
extern volatile int32_t LFSensorT;
extern volatile int32_t RFSensorT;
extern int32_t sensorScale;
extern int32_t alignErrorTolerance;

extern int32_t angle1;
extern int32_t angle2;
extern int32_t angle3;
extern int32_t angleT;
extern int32_t aSpeed1;
extern int32_t aSpeed2;
extern int32_t aSpeed3;
extern int32_t Outz;
extern int32_t aSpeed3Threshold;

extern float gyroScale1;
extern float gyroScale2;
extern float gyroScale3;
extern float outZOffset;
extern float rateOffset;

extern int32_t scaledRate1;
extern int32_t scaledRate2;
extern int32_t scaledOutz;

extern int32_t LDMidVal;
extern int32_t RDMidVal;
extern int32_t leftWallThreshold;
extern int32_t rightWallThreshold;
extern int32_t frontWallThresholdL;
extern int32_t frontWallThresholdR;
extern int32_t approachThresholdL;
extern int32_t approachThresholdR;

extern int32_t alignTime;

void readVoltage(void);
void readIRSensors(void);
int32_t getSensorError(void);
void readGyro(void);
void calibrateIRSensors(void);
void calibrateGyros(void);
void calibrateGyrosTemp(void);

#endif
