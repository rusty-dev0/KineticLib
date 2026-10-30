#pragma once

#include "pico/stdlib.h"

struct pidController {
    double kp;
    double ki;
    double kd;

    double integral;
    double prevError;

    double outputMin;
    double outputMax;

    bool init;
};

int initializePidController(struct pidController *pidControllerObject, double kp, double ki, double kd, double outputMin, double outputMax);
double updatePidController(struct pidController *pidControllerObject, double setpoint, double measured, double deltaTime);