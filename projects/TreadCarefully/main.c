#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "Direction.h"
#include "Motor.h"
#include "Encoder.h"

#define MAX_SPEED 100

volatile double previousPositionA = 0;
volatile double displacementA = 0;

volatile double previousTime = 0;
volatile double deltaTime = 0;

bool handleTimer(struct repeating_timer *t) {
    displacementA = encoderPositionA - previousPositionA;
    deltaTime = time_us_64() - previousTime;
    previousPositionA = encoderPositionA;
    previousTime = time_us_64();

    return true;
}

void beginFeedback() {
    static struct repeating_timer timer;
    add_repeating_timer_ms(-50, handleTimer, NULL, &timer);
}

void printSpeed() {
    double speed = (displacementA / 720) / (deltaTime) * 6.0e7; // should be in RPM
    printf("motor A is going at %lf RPM\n", speed);
}

int main() {
    stdio_init_all();
    initializeEncoders();

    if (cyw43_arch_init()) {
        printf("WIFI FAILED. WIFI FAILED.\n");
        return -1;
    }

    beginFeedback();

    sleep_ms(1000);

    gpio_init(KL_LED_PIN);
    gpio_set_dir(KL_LED_PIN, GPIO_OUT);

    gpio_init(KL_STBY_PIN);
    gpio_set_dir(KL_STBY_PIN, GPIO_OUT);
    gpio_put(KL_STBY_PIN, 1); // wake up motor driver

    bool state = false;

    struct pwmChannel pwmA = {0}, pwmB = {0};
    pwmChannel_init(&pwmA, KL_PWMA_PIN, 20000); // 20kHz which is safe for the motor driver
    pwmChannel_init(&pwmB, KL_PWMB_PIN, 20000);

    struct direction motorDirectionA = {0}, motorDirectionB = {0};
    initDirection(&motorDirectionA, KL_AIN1_PIN, KL_AIN2_PIN);
    initDirection(&motorDirectionB, KL_BIN1_PIN, KL_BIN2_PIN);

    setHeading(&motorDirectionA, CLOCKWISE);
    setHeading(&motorDirectionB, COUNTERCLOCKWISE);


    double desired_speed = 45.0; //(RPM) change this later based on UART
    double calculated_speed = desired_speed / (double)MAX_SPEED; 

    // slowly reach desired speed
    for (double s = 0; s < calculated_speed; s+=0.1) {
        state = !state;
        gpio_put(KL_LED_PIN, state);
        pwmChannel_setDutyCycle(&pwmA, s);
        pwmChannel_setDutyCycle(&pwmB, s);
        sleep_ms(100);
        printSpeed();
        sleep_ms(400);
    }

    // now go to speed
    pwmChannel_setDutyCycle(&pwmA, calculated_speed);
    pwmChannel_setDutyCycle(&pwmB, calculated_speed);
    sleep_ms(100);

    for (int i = 0; i < 10; i++) {
        printSpeed();
        sleep_ms(500);
    }

    pwmChannel_halt(&pwmA);
    pwmChannel_halt(&pwmB);
    sleep_ms(50);
    setHeading(&motorDirectionA, ZERO);
    setHeading(&motorDirectionB, ZERO);
    sleep_ms(450);

    setHeading(&motorDirectionA, COUNTERCLOCKWISE);
    setHeading(&motorDirectionB, CLOCKWISE);

    for (double speed = 1; speed > 0; speed-=0.1) {
        state = !state;
        gpio_put(KL_LED_PIN, state);
        pwmChannel_setDutyCycle(&pwmA, speed);
        pwmChannel_setDutyCycle(&pwmB, speed);
        sleep_ms(100);
        printSpeed();
        sleep_ms(400);
    }

    pwmChannel_halt(&pwmA);
    pwmChannel_halt(&pwmB);
    sleep_ms(50);
    setHeading(&motorDirectionA, ZERO);
    setHeading(&motorDirectionB, ZERO);
    sleep_ms(450);

    printSpeed();

    return 0;
}