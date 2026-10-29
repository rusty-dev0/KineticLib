#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "Motor.h"

int main() {
    stdio_init_all();

    gpio_init(KL_LED_PIN);
    gpio_set_dir(KL_LED_PIN, GPIO_OUT);

    bool state = 0;

    for (uint32_t counter = 0; ; counter++) {
        gpio_put(KL_LED_PIN, state);
        state = !state;
        printf("hello, Pi! This is message #%d\n", counter);
        sleep_ms(1000);
    }

    return 0;
}