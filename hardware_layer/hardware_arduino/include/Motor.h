#ifndef __Motor_H_INCLUDED__
#define __Motor_H_INCLUDED__

#include "Arduino.h"

// Motor being used are brushed DC motors
// Motor Driver: L298

class Motor
{
    public:
        Motor(int pwm_pin, int motor_pinA, int motor_pinB);
        void setup();
        void spin(int pwm);

    private:
        int pwm_pin_;
        int motor_pinA_;
        int motor_pinB_;
};

#endif
