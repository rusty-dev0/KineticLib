#pragma once

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define KL_A_ENCA 10
#define KL_A_ENCB 11

#define KL_B_ENCA 12
#define KL_B_ENCB 13

extern volatile int encoderPositionA;
extern volatile int encoderPositionB;

void handleEncoderUpdateA(uint gpio, uint32_t events);
void handleEncoderUpdateB(uint gpio, uint32_t events);
void initializeEncoders();