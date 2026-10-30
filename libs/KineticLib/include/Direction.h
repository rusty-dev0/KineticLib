#pragma once

#include "pico/stdlib.h"

enum Direction {
    CLOCKWISE,
    COUNTERCLOCKWISE,
    HALT,
    ZERO
};

struct direction {
    uint pin1;
    uint pin2;
    bool init;
};

int initDirection(struct direction *directionObject, uint pin1, uint pin2);
int setHeading(struct direction *directionObject, enum Direction d);