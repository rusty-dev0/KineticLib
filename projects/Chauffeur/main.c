#include <stdio.h>
#include <math.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "Direction.h"
#include "Encoder.h"
#include "Motor.h"
#include "PID.h"

void setupBoard() {
    gpio_init(KL_LED_PIN);
    gpio_set_dir(KL_LED_PIN, GPIO_OUT);

    gpio_init(KL_STBY_PIN);
    gpio_set_dir(KL_STBY_PIN, GPIO_OUT);
    gpio_put(KL_STBY_PIN, 1); // wake up motor driver
}

int main() {
    stdio_init_all(); // this automatically sets up uart0 peripheral so I don't need to do it manually
    initializeRPM();
    setupBoard();

    sleep_ms(1000);

    struct pwmChannel pwmA = {0}, pwmB = {0};
    pwmChannel_init(&pwmA, KL_PWMA_PIN, 20000); // 20kHz which is safe for the motor driver
    pwmChannel_init(&pwmB, KL_PWMB_PIN, 20000);

    struct direction motorDirectionA = {0}, motorDirectionB = {0};
    initDirection(&motorDirectionA, KL_AIN1_PIN, KL_AIN2_PIN);
    initDirection(&motorDirectionB, KL_BIN1_PIN, KL_BIN2_PIN);

    struct pidController pidA = {0}, pidB = {0};
    initializePidController(&pidA, 0.0045, 0.001, 0, 0.1, 1); // 0.000001
    initializePidController(&pidB, 0.0045, 0.001, 0, 0.1, 1);

    double targetSpeed = 0;
    bool inRotation = false;
    bool stop = false;

    uint64_t prevTime = time_us_64();
    sleep_ms(150);

    while (true) {
        // wait for movement command
        if (uart_is_readable(uart0)) {
            char c = uart_getc(uart0);
            // structure = 8 bits: xxxrs101
            // the 101 at the end right there is the magic
            // the r bit is for rotation
            // the s bit is for a complete stop
            // the x bits are for speed
                        
            // validate all three magic bits
            if ((c & 0b111) == 0b101) {

                // stop command
                if (c & (1 << 3)) {
                    stop = true;

                    pwmChannel_setDutyCycle(&pwmA, 0);
                    pwmChannel_setDutyCycle(&pwmB, 0);

                    setHeading(&motorDirectionA, ZERO);
                    setHeading(&motorDirectionB, ZERO);

                    resetPidController(&pidA);
                    resetPidController(&pidB);

                    continue;
                }

                uint8_t speedBits = (c >> 5) & 0b111;
                double newTargetSpeed = (speedBits / 7.0) * 95.0;

                bool newRotation = (c & (1 << 4)) != 0;

                // only reset if the command actually changes
                if (stop || newRotation != inRotation ||
                    newTargetSpeed != targetSpeed) {

                    resetPidController(&pidA);
                    resetPidController(&pidB);

                    prevTime = time_us_64();
                }

                stop = false;
                inRotation = newRotation;
                targetSpeed = newTargetSpeed;

                if (inRotation) {
                    setHeading(&motorDirectionA, CLOCKWISE);
                    setHeading(&motorDirectionB, CLOCKWISE);
                } else {
                    setHeading(&motorDirectionA, CLOCKWISE);
                    setHeading(&motorDirectionB, COUNTERCLOCKWISE);
                }
            }
        }

        uint64_t currTime = time_us_64();
        double deltaTime = (currTime - prevTime) / 1.0e6;
        prevTime = currTime;

        if (stop) continue;

        double motorRPMs[2];

        getRPMs(motorRPMs);

        motorRPMs[0] = fabs(motorRPMs[0]);
        motorRPMs[1] = fabs(motorRPMs[1]);

        double dutyA = updatePidController(&pidA, targetSpeed, motorRPMs[0], deltaTime);
        double dutyB = updatePidController(&pidB, targetSpeed, motorRPMs[1], deltaTime);

        pwmChannel_setDutyCycle(&pwmA, dutyA);
        pwmChannel_setDutyCycle(&pwmB, dutyB);

        // printSpeed();

        sleep_ms(150);
    }
}