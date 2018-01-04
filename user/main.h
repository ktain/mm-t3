#ifndef MAIN_H
#define MAIN_H

#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "userconfig.h"
#include "home.h"
#include "setup.h"
#include "buzzer.h"
#include "delay.h"
#include "motorcontrol.h"
#include "encoder.h"
#include "sensor.h"
#include "button.h"
#include "model.h"
#include "view.h"
#include "controller.h"
#include "search.h"
#include "speedrun.h"
#include "display.h"
#include "flash.h"
#include "test.h"

void systick(void);


#endif
