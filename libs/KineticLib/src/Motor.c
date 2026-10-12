#include "Motor.h"
#include "hardware/pwm.h"

/*
Error Codes:
 0: method was successfully completed.
-1: pwmObject failed to initialize because the object was previously initialized.
-2: method failed because pwmObject is not yet initialized.
-3: method failed because dutyCycle is not in range from 0 to 1.
-4: pwmObject failed to initialize because frequency was set to 0.
*/

int pwmChannel_init(struct pwmChannel *pwmObject, uint pin, uint freq) {
    if (pwmObject->init) return -1;
    if (freq == 0) return -4;
    
    uint wrap = 125000000 / freq - 1;

    return pwmChannel_rawInit(pwmObject, pin, wrap);
}

// @param wrap the max number of counts in the pico's counter before resetting back to 0
int pwmChannel_rawInit(struct pwmChannel *pwmObject, uint pin, uint wrap) {
    if (pwmObject->init) return -1;

    pwmObject->init = false;

    gpio_set_function(pin, GPIO_FUNC_PWM);

    uint slice_num = pwm_gpio_to_slice_num(pin);
    uint chan = pwm_gpio_to_channel(pin);

    pwm_set_wrap(slice_num, wrap);
    pwm_set_chan_level(slice_num, chan, 0);

    pwm_set_enabled(slice_num, true); // this should never be disabled, even when powering down the channel.
                                      // otherwise, the pico risks halting at a high state.

    pwmObject->pin = pin;
    pwmObject->wrap = wrap;
    pwmObject->setPoint = 0;
    pwmObject->slice_num = slice_num;
    pwmObject->chan = chan;
    pwmObject->init = true;

    return 0;
}

int pwmChannel_halt(struct pwmChannel *pwmObject) {
    if (!pwmObject->init) return -2;

    pwm_set_chan_level(pwmObject->slice_num, pwmObject->chan, 0);

    return 0;
}

// @param dutyCycle percentage of time in each cycle that the value is HIGH. express the percentage between 0 and 1
int pwmChannel_setDutyCycle(struct pwmChannel *pwmObject, double dutyCycle) {
    if (!pwmObject->init) return -2;
    if (dutyCycle < 0 || dutyCycle > 1) return -3;

    uint setPoint = dutyCycle * pwmObject->wrap;
    pwm_set_chan_level(pwmObject->slice_num, pwmObject->chan, setPoint);
    pwmObject->setPoint = setPoint;

    return 0;
}