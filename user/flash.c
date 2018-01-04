#include "main.h"

/* Base address of the Flash sectors */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

uint32_t startAddress = 0;	//0x8010000 is 64KB
				//0x8040000 is 256KB
				//0x8080000 is 512KB
				//0x80C0000 is 768KB

// Save maze data to sector 11, up to 128KB
void saveMaze(void) {
	startAddress = ADDR_FLASH_SECTOR_11; // start of sector 11, 896KB
	FLASH_Unlock();

	/* Clear All pending flags */
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	FLASH_EraseSector(FLASH_Sector_11, VoltageRange_3);

	// Save destination
	FLASH_ProgramHalfWord((startAddress), destinationX);
	FLASH_ProgramHalfWord((startAddress + 4), destinationY);
	startAddress += 8;

	// Save maze
	for(uint32_t i = 0; i < SIZEX; i++) {
		for(uint32_t j = 0; j < SIZEY; j++) {
			FLASH_ProgramHalfWord((startAddress + (i*SIZEY + j)*4), cell[i][j]);
		}
	}


	FLASH_Lock();
}

// Read maze data from sector 11
void loadMaze(void) {
	startAddress = ADDR_FLASH_SECTOR_11;	// start of sector 11, 896KB

	// Load destination
	destinationX = *(int16_t *)(startAddress);
	destinationY = *(int16_t *)(startAddress + 4);
	startAddress += 8;

	// Load maze
	for(uint32_t i = 0; i < SIZEX; i++) {
		for(uint32_t j = 0; j < SIZEY; j++) {
			cell[i][j] = *(int16_t *)(startAddress + (i*SIZEY + j)*4);
		}
	}
}

// Save sensor data to sector 10, up to 128KB
void saveSensorData(void) {
	startAddress = ADDR_FLASH_SECTOR_10;	// start of sector 10, 768KB
	FLASH_Unlock();

	/* Clear All pending flags */
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	FLASH_EraseSector(FLASH_Sector_10, VoltageRange_3);

	// Save LF sensor data
	uint32_t i;
	for (i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		FLASH_ProgramWord(startAddress + i*4, LFSensorLookup[i]);
	}

	// Save RF sensor data
	startAddress = startAddress + i*4;
	uint32_t j;
	for (j = 0; j < SENSOR_DATA_ARRAY_SIZE; j++) {
		FLASH_ProgramWord(startAddress + j*4, RFSensorLookup[j]);
	}

	// Save LD and RD sensor data
	startAddress = startAddress + j*4;
	FLASH_ProgramWord(startAddress, LDSensorLookup);
	j++;
	FLASH_ProgramWord(startAddress + j*4, RDSensorLookup);
}

// Load sensor data from sector 10
void loadSensorData(void) {

	// Load LF sensor data
	startAddress = ADDR_FLASH_SECTOR_10;	// start of sector 10, 768KB
	uint32_t i;
	for(i = 0; i < SENSOR_DATA_ARRAY_SIZE; i++) {
		LFSensorLookup[i] = *(int32_t *)(startAddress + i*4);
	}

	// Load RFSensor data
	startAddress = startAddress + i*4;
	uint32_t j;
	for(j = 0; j < SENSOR_DATA_ARRAY_SIZE; j++) {
		RFSensorLookup[j] = *(int32_t *)(startAddress + j*4);
	}

	// Load combined sensor data
	uint32_t k;
	for(k = 0; k < SENSOR_DATA_ARRAY_SIZE; k++) {
		sensorLookup[k] = (LFSensorLookup[k] + RFSensorLookup[k])/2;
	}

	// Load LD and RD sensor lookup
	startAddress = startAddress + j*4;
	LDSensorLookup = *(int32_t *)(startAddress);
	j++;
	RDSensorLookup = *(int32_t *)(startAddress + j*4);

}

// Save gyro data to sector 9, up to 128KB
void saveGyroData(void) {
	startAddress = ADDR_FLASH_SECTOR_9;	// start of sector 9, 640KB
	FLASH_Unlock();

	/* Clear All pending flags */
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	FLASH_EraseSector(FLASH_Sector_9, VoltageRange_3);

	// Save scaled steady state gyro constants
	FLASH_ProgramWord(startAddress, scaledRate1);
	FLASH_ProgramWord(startAddress + 4, scaledRate2);
	FLASH_ProgramWord(startAddress + 8, scaledOutz);
}


// Load gyro data from sector 9
void loadGyroData(void) {

	// Load gyro data
	startAddress = ADDR_FLASH_SECTOR_9;	// start of sector 9, 640KB
	
	// Load scaled steady state gyro constants
	scaledRate1 = *(int32_t *)(startAddress);
	scaledRate2 = *(int32_t *)(startAddress + 4);
	scaledOutz = *(int32_t *)(startAddress + 8);
}
