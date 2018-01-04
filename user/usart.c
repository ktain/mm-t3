#include "main.h"


struct __FILE { 
	int32_t handle;
};


FILE __stdout;
FILE __stdin;


void _sys_exit(int32_t x) {  
	x = x;  
}

int32_t fputc(int32_t ch, FILE *f) { 
	USART_SendData(USART1, (uint8_t)ch); 
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	return ch; 
} 

int32_t fgetc(FILE *f) {
	uint16_t temp;
	while(!(USART1->SR & USART_SR_RXNE));
	temp = (uint16_t)(USART1->DR & (uint16_t)0x01FF);
	return temp;
}
