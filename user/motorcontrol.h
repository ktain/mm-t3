#ifndef PWM_H
#define PWM_H   

struct turnProfile {
	float d1;
	float t1;
	float t2;
	float t3;
	float d2;
	float startSpeed;
	float speedX;
	float endSpeed;
	int32_t speedW;
	int32_t accW;
	int32_t decW;
	float kpW13;
	float kdW13;
	float kpW2;
	float kdW2;
};

struct pivotTurnProfile {
	int32_t speedW;
	int32_t angle;
	int32_t delay;
};

extern int32_t speedRunProfile;
extern int32_t searchProfile;

extern bool useMotorControl1;
extern bool useMotorControl2;
extern bool useIRSensors;
extern bool useOnlyGyroFeedback;
extern bool useOnlyEncFeedback;
extern bool useAngleFeedback;
extern bool useAlignment;
extern bool useFrontWallCalibration;
extern bool useCrashDetection;
extern bool useAngularCorrection;

extern int32_t maxPwm;
extern int32_t maxBoostPwm;
extern int32_t maxFanPwm;
extern float alignSpeed;
extern float calibrateSpeed;
extern float centerToBackDist;	// Distance in mm from center of mass to back
extern float cellDistance;
extern float frontEdgeDist;	// Distance in mm from front wall to front edge 
extern float frontEdgeDistTurn;	// Distance in mm from front wall to front edge before curve turn
extern float pivotOffsetBefore;
extern float pivotOffsetAfter;
extern float counts_per_360;
extern float wheelDistance;
extern float wheelDistanceL;
extern float wheelDistanceR;
extern float angleScale;
extern int32_t alignSensorScale;
extern int32_t leftPwmTotal;
extern int32_t rightPwmTotal;

extern float encCountLeft;
extern float encCountRight;
extern float encCount;
extern float oldEncCountLeft;
extern float oldEncCountRight;
extern float encChangeLeft;
extern float encChangeRight;
extern float encChange;
extern float distanceLeft;

extern float targetSpeedX;
extern float targetSpeedW;
extern float curSpeedX;
extern float curSpeedW;
extern float encFeedbackX;
extern float encFeedbackW;
extern float gyroFeedback;
extern float errorX;
extern float errorW;
extern float oldErrorX;
extern float oldErrorW;

extern float kpX;
extern float kdX;
extern float kpW;
extern float kdW;

extern float stopSpeed;
extern float searchSpeed;
extern float turnSpeed;
extern float traceSpeed;
extern float straightSpeed;
extern float diagSpeed;

extern float accX;
extern float decX;
extern float accW;
extern float decW;
extern float alignAccX;
extern float alignDecX;
extern float alignAccW;
extern float alignDecW;
extern float alignKdW;
extern float alignKpW;
extern float calibrateAccX;
extern float calibrateDecX;
extern float straightAccX;
extern float straightDecX;
extern float straightAccW;
extern float straightDecW;
extern float straightKdX;
extern float straightKpX;
extern float straightKdW;
extern float straightKpW;
extern float diagAccX;
extern float diagDecX;
extern float diagAccW;
extern float diagDecW;
extern float diagKdX;
extern float diagKpX;
extern float diagKdW;
extern float diagKpW;

extern float setSpeedX;
extern float setSpeedW;
extern float setSpeedL;
extern float setSpeedR;

extern int32_t pwmLimitCounter;
extern int32_t pwmLimitTime;
extern int32_t crashWallThreshold;
extern int32_t pivotTimeLimit;

void updateSpeed1(void);
void updateSpeed2(void);
float getDecNeeded(float d, float Vi, float Vf);
void setLeftPwm(int32_t pwm);
void setRightPwm(int32_t pwm);
void setLeftBoostPwm(int32_t pwm);
void setRightBoostPwm(int32_t pwm);
void setFanPwm(int32_t pwm);
void enableMotorControl(void);
void disableMotorControl(void);
void resetMotorParameters(void);

void moveStraight(float cells, float maxSpeed, float endSpeed);
void moveStraightTurn(float cells, float maxSpeed, float endSpeed);
void calibrateTurn(float maxSpeed, float endSpeed);
void moveDiagonal(float count, float maxSpeed, float endSpeed);
void turn(struct turnProfile profile);
void turnDiag(struct turnProfile profile);
void pivotTurn(struct pivotTurnProfile profile);
void align(void);

float counts_to_mm(float counts);
float mm_to_counts(float speed);
float counts_to_deg(float counts);
float deg_to_counts(float deg);

extern int32_t hugTime;
extern int32_t hugPwm;
extern float hugSensorScale;

#endif
