#pragma once

#include "pico/stdlib.h"

#define KL_PWMA_PIN 2
#define KL_AIN1_PIN 3
#define KL_AIN2_PIN 4

#define KL_STBY_PIN 5

#define KL_PWMB_PIN 6
#define KL_BIN1_PIN 7
#define KL_BIN2_PIN 8

#define KL_LED_PIN 9

struct pwmChannel {
    uint pin;
    uint wrap;
    uint setPoint;
    uint slice_num;
    uint chan;
    bool init;
};

int pwmChannel_rawInit(struct pwmChannel *pwmObject, uint pin, uint wrap);
int pwmChannel_init(struct pwmChannel *pwmObject, uint pin, uint freq);
int pwmChannel_halt(struct pwmChannel *pwmObject);
int pwmChannel_setDutyCycle(struct pwmChannel *pwmObject, double dutyCycle);