#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#define PICO_LED_PIN CYW43_WL_GPIO_LED_PIN
#define KINETICBOARD_LED_PIN 9

int main()
{
    stdio_init_all();

    if (cyw43_arch_init()) {
        printf("WIFI FAILED. WIFI FAILED.\n");
        return -1;
    }

    sleep_ms(1000);

    gpio_init(KINETICBOARD_LED_PIN);
    gpio_set_dir(KINETICBOARD_LED_PIN, GPIO_OUT);

    bool state = 0;

    printf("BOARD READY. BOARD READY.\n");

    sleep_ms(1000);

    while (true) {
        gpio_put(KINETICBOARD_LED_PIN, state);
        state = !state;
        cyw43_arch_gpio_put(PICO_LED_PIN, state);
        sleep_ms(250);
    }
}
