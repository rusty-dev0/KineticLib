#include "Direction.h"

/*
Error Codes:
 0: method was successfully completed.
-1: direction failed to initialize because the object was previously initialized.
-2: method failed because direction is not yet initialized.
-3: some weird shit happened in setHeading's switch case that bypassed everything before
*/

int initDirection(struct direction *directionObject, uint pin1, uint pin2) {
    if (directionObject->init) return -1;

    directionObject->init = true;

    gpio_init(pin1);
    gpio_init(pin2);

    gpio_set_dir(pin1, GPIO_OUT);
    gpio_set_dir(pin2, GPIO_OUT);

    directionObject->pin1 = pin1;
    directionObject->pin2 = pin2;

    return 0;
}

int setHeading(struct direction *directionObject, enum Direction d) {
    if (!directionObject->init) return -2;

    switch (d) {
        case CLOCKWISE:
            gpio_put(directionObject->pin1, 1);
            gpio_put(directionObject->pin2, 0);
            break;
        case COUNTERCLOCKWISE:
            gpio_put(directionObject->pin1, 0);
            gpio_put(directionObject->pin2, 1);
            break;
        case HALT:
            gpio_put(directionObject->pin1, 1);
            gpio_put(directionObject->pin2, 1);
            break;
        case ZERO:
            gpio_put(directionObject->pin1, 0);
            gpio_put(directionObject->pin2, 0);
            break;
        default:
            return -3;
            break;
    }

    return 0;
}