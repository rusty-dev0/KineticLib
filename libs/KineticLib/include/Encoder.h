#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define KL_A_ENCA 10
#define KL_A_ENCB 11

#define KL_B_ENCA 12
#define KL_B_ENCB 13

extern volatile int encoderPositionA;
extern volatile int encoderPositionB;

void handleEncoderUpdate();
void handleEncoderUpdateA();
void handleEncoderUpdateB();
void initializeEncoders();