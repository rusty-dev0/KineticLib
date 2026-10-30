#include "Encoder.h"

volatile int encoderPositionA = 0;
volatile int encoderPositionB = 0;

volatile double motorRPM_A = 0;
volatile double motorRPM_B = 0;

volatile int previousPositionA = 0;
volatile int previousPositionB = 0;
volatile uint64_t previousTime = 0;

bool encodersInitialized = false;

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

int initializeEncoders() {

    if (encodersInitialized) return -1;

    int encPins[4] = {KL_A_ENCA, KL_A_ENCB, KL_B_ENCA, KL_B_ENCB};

    for (int i = 0; i < 4; i++) {
        gpio_init(encPins[i]);
        gpio_set_dir(encPins[i], GPIO_IN);
    }

    gpio_set_irq_enabled_with_callback(KL_A_ENCA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &handleEncoderUpdate);
    gpio_set_irq_enabled(KL_B_ENCA, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

}

bool handleTimer(struct repeating_timer *t) {
    uint64_t currentTime = time_us_64();

    uint32_t irqState = save_and_disable_interrupts(); // freeze so both encoderPositions are sampled at the same moment in time
    
    int32_t currentEncoderPositionA = encoderPositionA;
    int32_t currentEncoderPositionB = encoderPositionB;

    restore_interrupts(irqState);

    double displacementA = currentEncoderPositionA - previousPositionA;
    double displacementB = currentEncoderPositionB - previousPositionB;
    double deltaTime = currentTime - previousTime;

    if (deltaTime > 0) {
        motorRPM_A = (displacementA / COUNTS_PER_REV) * (6.0e7 / deltaTime);
        motorRPM_B = (displacementB / COUNTS_PER_REV) * (6.0e7 / deltaTime);
    }

    previousPositionA = currentEncoderPositionA;
    previousPositionB = currentEncoderPositionB;
    previousTime = currentTime;

    return true;
}

void initializeRPM() {

    if (!encodersInitialized) initializeEncoders();

    static struct repeating_timer timer;

    uint32_t irqState = save_and_disable_interrupts();

    previousPositionA = encoderPositionA;
    previousPositionB = encoderPositionB;

    restore_interrupts(irqState);

    previousTime = time_us_64();

    add_repeating_timer_ms(-50, handleTimer, NULL, &timer);
}

void getRPMs(double *arr) {
    // THIS CAN ONLY WORK IF THE ARRAY IS SIZE 2 OR GREATER
    // USE WITH CAUTION!!
    uint32_t irqState = save_and_disable_interrupts();

    arr[0] = motorRPM_A;
    arr[1] = motorRPM_B;
    
    restore_interrupts(irqState);
}