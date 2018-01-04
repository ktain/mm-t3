#ifndef SETUP_H
#define SETUP_H

// LED Pinout
#define LED1_ON		GPIO_SetBits(GPIOA, GPIO_Pin_7)
#define LED2_ON		GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define LED3_ON		GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define LED1_OFF	GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define LED2_OFF	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define LED3_OFF	GPIO_ResetBits(GPIOC, GPIO_Pin_4)

#define ALL_LED_ON	LED1_ON; LED2_ON; LED3_ON;
#define ALL_LED_OFF	LED1_OFF; LED2_OFF; LED3_OFF;

// Emitter Pinout
#define LF_EM_ON1	GPIO_SetBits(GPIOC, GPIO_Pin_10)
#define LF_EM_ON2	GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define RF_EM_ON1	GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define RF_EM_ON2	GPIO_SetBits(GPIOD, GPIO_Pin_2)
#define LD_EM_ON	GPIO_SetBits(GPIOC, GPIO_Pin_11)
#define RD_EM_ON	GPIO_SetBits(GPIOC, GPIO_Pin_12)

#define LF_EM_OFF1	GPIO_ResetBits(GPIOC, GPIO_Pin_10)
#define LF_EM_OFF2	GPIO_ResetBits(GPIOA, GPIO_Pin_12)
#define RF_EM_OFF1	GPIO_ResetBits(GPIOB, GPIO_Pin_4)
#define RF_EM_OFF2	GPIO_ResetBits(GPIOD, GPIO_Pin_2)
#define LD_EM_OFF	GPIO_ResetBits(GPIOC, GPIO_Pin_11)
#define RD_EM_OFF	GPIO_ResetBits(GPIOC, GPIO_Pin_12)
#define ALL_EM_OFF	LF_EM_OFF1; LF_EM_OFF2; RF_EM_OFF1; RF_EM_OFF2; LD_EM_OFF; RD_EM_OFF;


// ADC Pinout
#define read_Voltage	(readADC(ADC1, 15, ADC_SampleTime_15Cycles)*VOLTAGEMULTIPLIER)
#define read_LF_Sensor	readADC(ADC1, 13, ADC_SampleTime_15Cycles)
#define read_LD_Sensor	readADC(ADC1, 12, ADC_SampleTime_15Cycles)
#define	read_RD_Sensor	readADC(ADC1, 10, ADC_SampleTime_15Cycles)
#define read_RF_Sensor	readADC(ADC1, 11, ADC_SampleTime_15Cycles)
#define	read_Outz	readADC(ADC1, 9,  ADC_SampleTime_15Cycles)
#define	read_Vref	readADC(ADC1, 8,  ADC_SampleTime_15Cycles)
#define read_Rate1	readADC(ADC1, 2,  ADC_SampleTime_15Cycles)
#define read_Rate2	readADC(ADC1, 3,  ADC_SampleTime_15Cycles)
#define read_Temp1	readADC(ADC1, 5,  ADC_SampleTime_15Cycles)
#define read_Temp2	readADC(ADC1, 4,  ADC_SampleTime_15Cycles)

// Bluetooth Pinout
#define OP4_ON		GPIO_SetBits(GPIOC, GPIO_Pin_9)
#define OP5_ON		GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define OP4_OFF		GPIO_ResetBits(GPIOC, GPIO_Pin_9)
#define OP5_OFF		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define BT_ON		GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define BT_OFF		GPIO_ResetBits(GPIOB, GPIO_Pin_12)

// Display Pinout
#define RS_1	GPIO_SetBits(GPIOB, GPIO_Pin_14)	//select control register
#define RS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_14)	//select data register
#define CE_1	GPIO_SetBits(GPIOC, GPIO_Pin_6)		//disable data writing, latch on
#define CE_0	GPIO_ResetBits(GPIOC, GPIO_Pin_6)	//enable date writing

// Buzzer Timers
#define beep_on		TIM3->CCR1 = (TIM3->ARR+1)/BEEPVOLUME
#define beep_off	TIM3->CCR1 = 0

// Motor Timers
#define LPWMB TIM4->CCR4
#define LPWMA TIM4->CCR3
#define RPWMB TIM4->CCR2
#define RPWMA TIM4->CCR1
#define FANPWM TIM1->CCR4
#define LEFTBOOSTPWM TIM8->CCR3
#define RIGHTBOOSTPWM TIM8->CCR2

void setup(void);
void systick_setup(void);
void led_setup(void);
void button_setup(void);
void buzzer_setup(void);
void fan_setup(void);
void encoder_setup(void);
void bluetooth_setup(void);
void sensor_setup(void);
void motor_setup(void);
void boostmotor_setup(void);
void display_setup(void);
uint16_t readADC(ADC_TypeDef* ADCx, uint8_t channel, uint8_t sampleTime);


#endif
