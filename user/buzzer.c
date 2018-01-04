#include "main.h"

// https://jpsoft.com/help/beep.htm
#define C3 1046
#define C3s 1108
#define D3 1175
#define D3s 1244
#define E3 1318
#define F3 1397
#define F3s 1480
#define G3 1568
#define G3s 1664
#define A3 1760
#define A3s 1866
#define B3 1973

#define C4 2096
#define C4s 2217
#define D4 2349
#define D4s 2489
#define E4 2637
#define F4 2794
#define F4s 2960
#define G4 3136
#define G4s 3322
#define A4 3520
#define A4s 3729
#define B4 3951

#define C5 4096
#define C5s 4434
#define D5 4698
#define D5s 4978
#define E5 5346
#define F5 5588
#define F5s 5920
#define G5 6272
#define G5s 6644
#define A5 7040
#define A5s 7458
#define B5 7902

struct tune tune_silent = {0};

struct tune tune_birthday = {
	125, C4, 250,	// 0
	125, C4, 125,
	500, D4, 125,
	500, C4, 125,
	500, F4, 125,
	1000, E4, 250,
	
	125, C4, 250,	// 6
	125, C4, 125,
	500, D4, 125,
	500, C4, 125,
	500, G4, 125,
	1000, F4, 250,
	
	125, C4, 250,	// 12
	125, C4, 125,
	500, E5, 125,
	500, A4, 125,
	250, F4, 125,
	125, F4, 125,
	
	500, E4, 125,	// 18
	1000, D4, 250,
	125, A4, 250,
	125, A4, 125,
	500, A4, 125,
	500, F4, 125,
	500, G4, 125,
	1000, F4, 0,
	
	-1, 0, 0,	// do not repeat
};

struct tune tune_victory = {
	100, B4, 50,	// 0
	100, B4, 50,
	100, B4, 50,
	400, B4, 0,
	400, G4, 0,
	400, A4, 0,
	100, B4, 200,
	100, A4, 0,
	800, B4, 0,
	400, F4s, 0,
	
	400, E4, 0,
	400, F4s, 0,
	100, E4, 100,
	400, A4, 0,
	100, A4, 100,
	400, G4s, 0,
	100, A4, 100,
	400, G4s, 0,
	100, G4s, 100,
	400, F4s, 0,
	
	400, E4, 0, // 20
	400, D4s, 0,
	100, E4, 100,
	1600, C4s, 0,
	400, F4s, 0,
	400, E4, 0,
	400, F4s, 0,
	100, E4, 100,
	400, A4, 0,
	100, A4, 100,
	
	400, G4s, 0,
	100, A4, 100,
	400, G4s, 0,
	100, G4s, 100,
	400, F4s, 0,
	400, E4, 0,
	400, F4s, 0,
	100, A4, 100,
	1600, B4, 0,
	
	-1, 0, 0,	// do not repeat
};

struct tune tune_mario = {
	//1, 0, 500,	// delayed start
	
	100, E4, 50,	// 0
	100, E4, 150,
	100, E4, 150,
	100, C4, 50,
	100, E4, 150,
	100, G4, 350,
	100, G3, 350,
	200, C4, 150,
	200, G3, 150,
	200, E3, 150,
	
	200, A3, 50,
	100, B3, 150,
	150, A3s, 1,
	100, A3, 150,
	100, G3, 75,
	100, E4, 75,
	100, G4, 75, 
	100, A4, 150,
	100, F4, 50,
	100, G4, 150,
	
	100, E4, 150,	// 20
	100, C4, 50,
	100, D4, 50,
	100, B3, 250,
	200, C4, 150,
	200, G3, 150,
	200, E3, 150,
	200, A3, 50,
	100, B3, 150,
	150, A3s, 1,
	
	100, A3, 150,
	100, G3, 75,
	100, E4, 75,
	100, G4, 75,
	100, A4, 150,
	100, F4, 50,
	100, G4, 150,
	100, E4, 150,
	100, C4, 50,
	100, D4, 50,
	
	100, B3, 450,	// 40
	100, G4, 50,
	100, F4s, 50,
	100, F4, 50,
	100, D4s, 150,
	100, E4, 150,
	100, G4s, 50,
	100, A3, 50,
	100, C4, 150,
	100, A3, 50,
	
	100, C4, 50,
	100, D4, 250,
	100, G4, 50,
	100, F4s, 50,
	100, F4, 50,
	100, D4s, 150,
	100, E4, 150,
	100, C5, 150,
	100, C5, 50,
	100, C5, 550,
	
	100, G4, 50,	// 60
	100, F4s, 50,
	100, F4, 50,
	100, D4s, 150,
	100, E4, 150,
	100, G4s, 50,
	100, A3, 50,
	100, C4, 150,
	100, A3, 50,
	100, C4, 50,
	
	100, D4, 250,
	200, D4s, 150,
	200, D4, 150,
	200, C4, 650,
	100, C4, 50,
	100, C4, 150,
	100, C4, 150,
	100, C4, 50,
	100, D4, 150,
	100, E4, 35,
	
	150, C4, 35,	// 80
	150, A3, 35,
	100, G3, 350,
	100, C4, 50,
	100, C4, 150,
	100, C4, 150,
	100, C4, 50,
	100, D4, 50,
	100, E4, 350,
	100, A4, 150,
	
	100, G4, 250,
	100, C4, 50,
	100, C4, 150,
	100, C4, 150,
	100, C4, 50,
	100, D4, 150,
	150, E4, 35,
	150, C4, 35,
	150, A3, 35,
	100, G3, 350,
	
	0, 0, 0,	// repeat
};

volatile int32_t beepDuration;	// in ms
volatile int32_t tone_index = 0;
volatile struct tune* tune_cur = &tune_silent;

// Non-blocking beep
void beep(int32_t duration, int32_t freq) {
	if (freq > 0) {
		beep_on;
		TIM3->ARR = 84000000/139/(freq);
	}
	beepDuration = duration;
}


// Multiple beep, blocking
void beepn(int32_t count) {
	tune_cur = &tune_silent;
	for (int32_t i = 0; i < count; i++) {
		beep(BEEPDELAY, BEEPFREQUENCY);
		if (i < count)
			delay_ms(2*BEEPDELAY);
	}
}

void playMelody(void) {
	static int32_t count_ms = 0;
	static int32_t led_num = 1;
	
	if (count_ms <= 0 && tune_cur->tone[tone_index].length_ms != 0) {
		beep(tune_cur->tone[tone_index].length_ms, tune_cur->tone[tone_index].freq);
		count_ms = tune_cur->tone[tone_index].length_ms + tune_cur->tone[tone_index].pause_ms;
		tone_index++;
		
		if (led_num == 1) {
			LED1_ON;
			LED2_OFF;
			LED3_OFF;
		}
		else if (led_num == 2) {
			LED1_OFF;
			LED2_ON;
			LED3_OFF;
		}
		else if (led_num == 3) {
			LED1_OFF;
			LED2_OFF;
			LED3_ON;
		}
		
		if (led_num == 3)
			led_num = 1;
		else 
			led_num++;
		
	} else if (tune_cur->tone[tone_index].length_ms < 0) {
		tone_index = 0;
		tune_cur = &tune_silent;
	} else if (tune_cur->tone[tone_index].length_ms == 0) {
		tone_index = 0;
	}
	
		
	count_ms--;
	if (count_ms < 0) {
		count_ms = 0;
		ALL_LED_OFF;
	}
}

void playVictory(void) {
	tone_index = 0;
	tune_cur = &tune_victory;
}

void playMario(void) {
	tone_index = 0;
	tune_cur = &tune_mario;
}

void playBirthday(void) {
	tone_index = 0;
	tune_cur = &tune_birthday;
}

void playChirp(void) {
	tune_cur = &tune_silent;
	for(int32_t j = 0; j < 1; j++) {
		for (int32_t i = 12; i > 0; i--) {
			beep(100, 2000+300*i);
			delay_ms(5);
		}
		for (int32_t i = 0; i < 12; i++) {
			beep(100, 2000+300*i);
			delay_ms(5);
		}
	}
}

void playPrelude(void) {
	tune_cur = &tune_silent;
	for(int32_t j = 0; j < 1; j++) {
		for (int32_t i = 0; i < 12; i++) {
			beep(50, 2000+300*i);
			delay_ms(5);
		}
		for (int32_t i = 12; i > 0; i--) {
			beep(50, 2000+300*i);
			delay_ms(5);
		}
	}
}

void playSiren(void) {
	tune_cur = &tune_silent;
	for(int32_t j = 0; j < 10; j++) {
		for (int32_t i = 0; i < 12; i++) {
			beep(100, 2000+300*i);
			delay_ms(5);
		}
		for (int32_t i = 12; i > 0; i--) {
			beep(100, 2000+300*i);
			delay_ms(5);
		}
	}
}

void playLost(void) {
	tune_cur = &tune_silent;
	beepn(5);
}

void playInvalid(void) {
	tune_cur = &tune_silent;
	beep(70, 300);
	delay_ms(100);
	beep(200, 300);
}

void playDone(void) {
	tune_cur = &tune_silent;
	for (int32_t i = 12; i > 0; i--) {
		beep(50, 2000+300*i);
		delay_ms(5);
	}
}
 
