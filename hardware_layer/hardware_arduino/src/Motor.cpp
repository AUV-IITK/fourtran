#include "Arduino.h"
#include "../include/Motor.h"

Motor::Motor(int pwm_pin, int motor_pinA, int motor_pinB)
{
    pwm_pin_ = pwm_pin;
    motor_pinA_ = motor_pinA;
    motor_pinB_ = motor_pinB;
    current_sensor_pin_ = -1;
}

Motor::Motor(int pwm_pin, int motor_pinA, int motor_pinB, int current_sensor_pin)
{
    pwm_pin_ = pwm_pin;
    motor_pinA_ = motor_pinA;
    motor_pinB_ = motor_pinB;
    current_sensor_pin_ = current_sensor_pin;
}

void Motor::setup()
{
  pinMode(pwm_pin_, OUTPUT);
  pinMode(motor_pinA_, OUTPUT);
  pinMode(motor_pinB_, OUTPUT);
}

void Motor::readCurrent()
{
    if(current_sensor_pin_ != -1)
        val_ = analogRead(current_sensor_pin_);
}

void Motor::spin(int pwm)
{
    if(pwm > 0)
    {
        digitalWrite(motor_pinA_, HIGH);
        digitalWrite(motor_pinB_, LOW);
    }
    else if(pwm < 0)
    {
        digitalWrite(motor_pinA_, LOW);
        digitalWrite(motor_pinB_, HIGH);
    }
    else
    {
      digitalWrite(motor_pinA_, LOW);
      digitalWrite(motor_pinB_, LOW);
    }
    // inverse mapping in the motor driver
    analogWrite(pwm_pin_, 255 - abs(pwm));
}
