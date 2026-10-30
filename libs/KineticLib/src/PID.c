#include "PID.h"

/*
error codes:
 0 it worked
-1 failed to init bc pidControllerObject is already initialized
-2 failed because pidControllerObject is not initialized
*/

int initializePidController(struct pidController *pidControllerObject, double kp, double ki, double kd, double outputMin, double outputMax) {
    if (pidControllerObject->init) return -1;

    pidControllerObject->kp = kp;
    pidControllerObject->ki = ki;
    pidControllerObject->kd = kd;
    pidControllerObject->outputMin = outputMin;
    pidControllerObject->outputMax = outputMax;

    pidControllerObject->init = true;

    return 0;
}

double updatePidController(struct pidController *pidControllerObject, double setpoint, double measured, double deltaTime) {
    if (!pidControllerObject->init) return -2;

    double error = setpoint - measured;
    pidControllerObject->integral += error * deltaTime;
    double dedt = (error - pidControllerObject->prevError) / deltaTime;

    double output = pidControllerObject->kp * error + pidControllerObject->ki * pidControllerObject->integral + pidControllerObject->kd * dedt; 
    pidControllerObject->prevError = error;

    if (output > pidControllerObject->outputMax) output = pidControllerObject->outputMax;
    if (output < pidControllerObject->outputMin) output = pidControllerObject->outputMin;

    return output;
}