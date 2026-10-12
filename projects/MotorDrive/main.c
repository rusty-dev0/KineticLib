#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "Motor.h"

int main() {
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("WIFI FAILED. WIFI FAILED.\n");
        return -1;
    }

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

    gpio_init(KL_AIN1_PIN);
    gpio_set_dir(KL_AIN1_PIN, GPIO_OUT);

    gpio_init(KL_AIN2_PIN);
    gpio_set_dir(KL_AIN2_PIN, GPIO_OUT);

    gpio_init(KL_BIN1_PIN);
    gpio_set_dir(KL_BIN1_PIN, GPIO_OUT);

    gpio_init(KL_BIN2_PIN);
    gpio_set_dir(KL_BIN2_PIN, GPIO_OUT);
    
    gpio_put(KL_AIN1_PIN, 1);
    gpio_put(KL_AIN2_PIN, 0);
    gpio_put(KL_BIN1_PIN, 0);
    gpio_put(KL_BIN2_PIN, 1);

    for (double speed = 0; speed < 1; speed+=0.1) {
        state = !state;
        gpio_put(KL_LED_PIN, state);
        pwmChannel_setDutyCycle(&pwmA, speed);
        pwmChannel_setDutyCycle(&pwmB, speed);
        sleep_ms(500);
    }

    pwmChannel_halt(&pwmA);
    pwmChannel_halt(&pwmB);
    sleep_ms(500);

    gpio_put(KL_AIN1_PIN, 0);
    gpio_put(KL_AIN2_PIN, 1);
    gpio_put(KL_BIN1_PIN, 1);
    gpio_put(KL_BIN2_PIN, 0);

    for (double speed = 1; speed > 0; speed-=0.1) {
        state = !state;
        gpio_put(KL_LED_PIN, state);
        pwmChannel_setDutyCycle(&pwmA, speed);
        pwmChannel_setDutyCycle(&pwmB, speed);
        sleep_ms(500);
    }

    pwmChannel_halt(&pwmA);
    pwmChannel_halt(&pwmB);

    sleep_ms(500);

    return 0;
}