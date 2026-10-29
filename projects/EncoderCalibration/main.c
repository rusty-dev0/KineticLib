#include <stdio.h>
#include "pico/stdlib.h"
#include "Motor.h"
#include "Encoder.h"

int main() {
    stdio_init_all();

    initializeEncoders();

    while (1) {
        printf("Motor 1: %d\nMotor 2: %d\n\n", encoderPositionA, encoderPositionB);
        sleep_ms(500);
    }

    return 0;
}