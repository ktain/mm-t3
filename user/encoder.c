#include "main.h"


int32_t getRightEncCount(void) {
	return TIM2->CNT;
}

void resetRightEncCount(void) {
	TIM2->CNT = 0;
}

int32_t getLeftEncCount(void) {
	return TIM5->CNT;
}

void resetLeftEncCount(void) {
	TIM5->CNT = 0;
}

void setRightEncCount(int32_t count) {
	TIM2->CNT = count;
}

void setLeftEncCount(int32_t count) {
	TIM5->CNT = count;
}


