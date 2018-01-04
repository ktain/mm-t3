#ifndef DELAY_H
#define DELAY_H   

uint32_t micros(void);
uint32_t millis(void);
void delay_ms(uint32_t duration);
void delay_us(uint32_t duration);
void elapse_ms(uint32_t duration, uint32_t startTime);
void elapse_us(uint32_t duration, uint32_t startTime);

extern volatile uint32_t Micros;
extern volatile uint32_t Millis;

#endif
