#include <ros.h>
#include <std_msgs/String.h>
#include <Arduino.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <math.h>

#define Ainp2 2
#define Apwm 3
#define Ainp1 4
#define Binp2 5
#define Bpwm 6
#define Binp1 7
#define Cinp2 8
#define Cinp1 9
#define Cpwm 10
#define Dpwm 11
#define Dinp2 12
#define Dinp1 13

#define current1 A5
#define current2 A4
#define current3 A3
#define current4 A2

void move_forward(int pwm, bool if_forward)
{
  if(if_forward)
  {
    digitalWrite(Binp1, HIGH);
    digitalWrite(Binp2, LOW);
    digitalWrite(Cinp1, HIGH);
    digitalWrite(Cinp2, LOW);
    analogWrite(Bpwm, pwm);
    analogWrite(Cpwm, pwm);
  }
  else
  {
    pwm = abs(pwm);
    digitalWrite(Binp1, LOW);
    digitalWrite(Binp2, HIGH);
    digitalWrite(Cinp1, LOW);
    digitalWrite(Cinp2, HIGH);
    analogWrite(Bpwm, pwm);
    analogWrite(Cpwm, pwm);
  }
}


void pwm_forwardCB(std_msgs::Int32& msg)
{
  if(msg.data >= 0)
    move_forward(msg.data, true);
  else
    move_forward(msg.data, false);
}

ros::NodeHandle n;
ros::Subscriber<std_msgs::Int32> pwm_forward("pwm/forward", pwm_forwardCB);



void setup()
{
  n.initNode();

  pinMode(Ainp1, OUTPUT);
  pinMode(Apwm, OUTPUT);
  pinMode(Ainp2, OUTPUT);
  pinMode(Binp1, OUTPUT);
  pinMode(Bpwm, OUTPUT);
  pinMode(Binp2, OUTPUT);
  pinMode(Cinp1, OUTPUT);
  pinMode(Cpwm, OUTPUT);
  pinMode(Cinp2, OUTPUT);
  pinMode(Dinp1, OUTPUT);
  pinMode(Dpwm, OUTPUT);
  pinMode(Dinp2, OUTPUT);

  pinMode(current1, INPUT);
  pinMode(current2, INPUT);
  pinMode(current3, INPUT);
  pinMode(current4, INPUT);

  n.subscribe(pwm_forward);
  std_msgs::Int32 msg;
  msg.data = 0;
  pwm_forwardCB(msg);
}

void loop()
{
  delay(200);
  n.spinOnce();

}
