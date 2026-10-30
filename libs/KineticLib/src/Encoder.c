#include "Encoder.h"

extern volatile int encoderPositionA = 0;
extern volatile int encoderPositionB = 0;

void handleEncoderUpdate(uint gpio, uint32_t events) {
    if (gpio == KL_A_ENCA) {
        handleEncoderUpdateA();
    } else if (gpio == KL_B_ENCA) {
        handleEncoderUpdateB();
    }
}

void handleEncoderUpdateA() {
    if (gpio_get(KL_A_ENCA) != gpio_get(KL_A_ENCB)) {
        encoderPositionA++;
    } else {
        encoderPositionA--;
    }
}

void handleEncoderUpdateB() {
    if (gpio_get(KL_B_ENCA) != gpio_get(KL_B_ENCB)) {
        encoderPositionB++;
    } else {
        encoderPositionB--;
    }
}

void initializeEncoders() {

    int encPins[4] = {KL_A_ENCA, KL_A_ENCB, KL_B_ENCA, KL_B_ENCB};

    for (int i = 0; i < 4; i++) {
        gpio_init(encPins[i]);
        gpio_set_dir(encPins[i], GPIO_IN);
    }

    gpio_set_irq_enabled_with_callback(KL_A_ENCA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handleEncoderUpdate);
    gpio_set_irq_enabled(KL_B_ENCA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

}