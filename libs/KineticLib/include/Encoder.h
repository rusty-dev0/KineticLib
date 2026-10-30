#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"

#define KL_A_ENCA 10
#define KL_A_ENCB 11

#define KL_B_ENCA 12
#define KL_B_ENCB 13

#define MAX_SPEED 100
#define COUNTS_PER_REV 740

extern volatile int encoderPositionA;
extern volatile int encoderPositionB;

extern volatile double motorRPM_A;
extern volatile double motorRPM_B;

void handleEncoderUpdate();
void handleEncoderUpdateA();
void handleEncoderUpdateB();
int initializeEncoders();

bool handleTimer(struct repeating_timer *t);
void initializeRPM();