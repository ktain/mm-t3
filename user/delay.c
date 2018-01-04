#include "main.h"


uint32_t micros(void)
{
	return (Millis*1000 + 1000 - SysTick->VAL/168);
}

uint32_t millis(void)
{
	return Millis; // Will not overflow for 49 days
}

// Blocking
void delay_ms(uint32_t duration)
{
	uint32_t curt = Millis;
	while((millis() - curt) < duration);
}

// Blocking
void delay_us(uint32_t duration)
{
	/* Old delay 
	uint32_t curt = micros();
	while((micros() - curt) < duration);
	*/
	
	/* DWT delay */
	volatile uint32_t cycles = duration*168;
	volatile uint32_t start = 0;
	
	// Set the Debug Exception and Monitor Control Register
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= 1 ;	// Enable the counter
	DWT->CYCCNT = 0;	// Reset the counter

	while(DWT->CYCCNT < cycles) {
		;	// busy wait
	}
}  

// Used to delay for a remaining amount of time
void elapse_ms(uint32_t duration, uint32_t startTime)
{
	while((millis() - startTime) < duration);
}

// Used to delay for a remaining amount of time
void elapse_us(uint32_t duration, uint32_t startTime)
{
	/* Old
	while((micros() - startTime) < duration);
	*/
	
	delay_us(duration - (micros() - startTime));
}



