#include "main.h"

void systick(void) {
	
	
	// Prioritize constant interval functions
	readGyro();
	readIRSensors();
	readVoltage();
	
	if (useMotorControl2)
		updateSpeed2();	// control for pivot turns
	else if (useMotorControl1)
		updateSpeed1();	// default control

	// Refresh display
	if (!busy) {
		refreshOptions();
	}
	
	// Update melody
	playMelody();
	
	
	/*
	// Detect PWM saturation
	if (leftPwmTotal >= 999 || rightPwmTotal >= 999 || leftPwmTotal <= -999 || rightPwmTotal <= -999) {
		LED1_ON;
	}
	else {
		LED1_OFF;
	}
	
	// Crash detection
	if (aSpeed3 > aSpeed3Threshold || aSpeed3 < -aSpeed3Threshold)
		NVIC_SystemReset();
	
	// Stall prevention
	if (leftPwmTotal >= maxPwm || rightPwmTotal >= maxPwm || leftPwmTotal <= -maxPwm || rightPwmTotal <= -maxPwm ) {	// Reset if motor pwm saturates
		pwmLimitCounter++;
		if (pwmLimitCounter > pwmLimitTime) {
			NVIC_SystemReset();
		}
	}
	else if (pwmLimitCounter > 0) {
		pwmLimitCounter--;
	}
	*/
}


int32_t main(void) {
	loadSettings();	// Load settings from flash
	setup();	// Setup pin configurations
	initializeMaze();
	delay_ms(200);	// Allow voltage to settle after power on
	playPrelude();
	displayVoltage();
	
	while(1) {
		//printf("LF %d LD %d RD %d RF %d\r\n", LFSensorT, LDSensor, RDSensor, RFSensorT);
		//printf("LF %d LD %d RD %d RF %d LDMidVal %d RDMidVal %d\r\n", LFSensorT, LDSensor, RDSensor, RFSensorT, LDMidVal, RDMidVal);
	}
		
}
