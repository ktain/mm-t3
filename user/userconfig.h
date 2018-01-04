#ifndef USERCONFIG_H
#define USERCONFIG_H

// Maze Settings
#define SIZEX 16			// size of maze
#define SIZEY 16
#define MAX_DIST (SIZEX*SIZEY)	// max distance for flood search

// Calibration settings
#define SENSOR_DATA_ARRAY_SIZE 300 		// 1mm per element
#define CALIBRATE_IR_SENSOR_START_DELAY 3000 	// Time in ms before starting
#define CALIBRATE_GYRO_START_DELAY 200		// Time in ms before starting
#define CALIBRATE_GYRO_SAMPLE_NUM 100		// Number of samples
#define CALIBRATE_GYRO_SAMPLE_DELAY 1		// Delay between samples in ms

// Alignment settings
#define ANGULAR_CORRECTION_TIME 200		// Time in ms

// IR Sensor settings
#define EMITTER_ON_TIME 30			// Time in us
#define EMITTER_OFF_TIME 30			// Total time = 8*ON_TIME + 3*OFF_TIME

// Motor settings
#define MOTORPERIOD 999
#define MOTORPRESCALER 3 	// PWM frequency = 84000000 / (period+1) / (prescaler+1)
#define BOOSTMOTORPERIOD 999
#define BOOSTMOTORPRESCALER 7	// PWM frequency = 168000000 / (999+1) / (3+1) = 21000Hz = 21KHz
#define FANMOTORPERIOD 999
#define FANMOTORPRESCALER 7	// PWM frequency = 168000000 / (999+1) / (3+1) = 21000Hz = 21KHz
#define USESLOWDECAY 0		// 0 for fast decay

// Bluetooth settings
#define BAUDRATE 9600

// Display settings
#define DISPLAYDELAY 200	// Scroll speed in ms
#define DISPLAYBRIGHTNESS 1	// 0-15

// Voltage settings
#define VOLTAGEMULTIPLIER 13240/4096

// Home settings
#define NUMOPTIONS 20

// Buzzer settings
#define BEEPDELAY 70		// Delay is ms for blocking beep
#define BEEPFREQUENCY	2000	// Frequency for blocking beep
#define BEEPVOLUME 3		// 8 == 12.5%, 4 == 25%, 2 == 50% loudness

// Gyro Settings
#define USEGYRO1 1		// ADXRS620-1 top
#define USEGYRO2 0		// ADXRS620-2 bottom
#define USEGYRO3 1		// LY3200ALH
#define ASPEED1CUTOFF 2000	// Use GYRO3 if surpasses cutoff
#define ASPEED2CUTOFF 2000	// Use GYRO3 if surpasses cutoff
#define GYRO_MULTIPLIER 10000	// Result should fit in int32_t
#define GYRO_SAMPLE_NUM 100	// Gyro samples every ms
#define GYRO_SAMPLE_INTERVAL 1	// Time in us between samples


// Print settings
#define HIDESOUTH 1	// Hide south cell wall when printing
#define HIDEEAST 1	// Hide east cell wall when printing

void loadSettings (void);

#endif
