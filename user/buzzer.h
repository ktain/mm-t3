#ifndef BUZZER_H
#define BUZZER_H

extern volatile int32_t beepDuration;

struct tone {
	int32_t length_ms;
	int32_t freq;
	int32_t pause_ms;
};

struct tune {
	struct tone tone[110];
};

void beep(int32_t duration, int32_t freq);
void beepn(int32_t count);
void playMelody(void);
void playVictory(void);
void playBirthday(void);
void playMario(void);
void playChirp(void);
void playLost(void);
void playInvalid(void);
void playSiren(void);
void playPrelude(void);
void playDone(void);

#endif
