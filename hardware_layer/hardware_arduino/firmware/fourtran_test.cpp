#if (ARDUINO >= 100)
    #include <Arduino.h>
#else
    #include <WProgram.h>
#endif

#include "ros.h"
#include "ros/time.h"
#include <std_msgs/Int32.h>

#include "../include/Motor.h"
#include "../include/FourTranConfig.h"

#define COMMAND_RATE 15 //hz
#define DEBUG_RATE 5

Motor motor1(MOTOR1_PWM, MOTOR1_IN_A, MOTOR1_IN_B);		//defining instances of motors
Motor motor2(MOTOR2_PWM, MOTOR2_IN_A, MOTOR2_IN_B);
Motor motor3(MOTOR3_PWM, MOTOR3_IN_A, MOTOR3_IN_B);
Motor motor4(MOTOR4_PWM, MOTOR4_IN_A, MOTOR4_IN_B);

float PWM_MAX = pow(2, PWM_BITS) - 1;

//callback function prototypes
void motor1_PWM_callback(const std_msgs::Int32& msg);
void motor2_PWM_callback(const std_msgs::Int32& msg);
void motor3_PWM_callback(const std_msgs::Int32& msg);
void motor4_PWM_callback(const std_msgs::Int32& msg);

ros::NodeHandle nh;
ros::Subscriber<std_msgs::Int32> motor1_pwm_sub("/pwm/motor1", &motor1_PWM_callback);
ros::Subscriber<std_msgs::Int32> motor2_pwm_sub("/pwm/motor2", &motor2_PWM_callback);
ros::Subscriber<std_msgs::Int32> motor3_pwm_sub("/pwm/motor3", &motor3_PWM_callback);
ros::Subscriber<std_msgs::Int32> motor4_pwm_sub("/pwm/motor4", &motor4_PWM_callback);

void setup()
{
    // setup motor hardware
    motor1.setup();
    motor2.setup();
    motor3.setup();
    motor4.setup();

    // setup ROS
    nh.initNode();
    nh.getHardware()->setBaud(57600);
    nh.subscribe(motor1_pwm_sub);
    nh.subscribe(motor2_pwm_sub);
    nh.subscribe(motor3_pwm_sub);
    nh.subscribe(motor4_pwm_sub);

    while (!nh.connected())
    {
        nh.spinOnce();
    }
    nh.loginfo("FourTran CONNECTED");
    delay(1);
}

void loop()
{
    delay(1000);  //call all the callbacks waiting to be called
    nh.spinOnce();
}

//callback function prototypes
void motor1_PWM_callback(const std_msgs::Int32& msg)
{
  motor1.spin(msg.data);
  nh.loginfo("Motor1 data received");
  // nh.logininfo("Motor1 data: %d",msg.data);
}
void motor2_PWM_callback(const std_msgs::Int32& msg)
{
  motor2.spin(msg.data);
  nh.loginfo("Motor2 data received");
  // nh.loginfo("Motor2 data: %d", msg.data);

}
void motor3_PWM_callback(const std_msgs::Int32& msg)
{
  motor3.spin(msg.data);
  nh.loginfo("Motor3 data received");
  // nh.loginfo("Motor3 data: %d", msg.data);
}
void motor4_PWM_callback(const std_msgs::Int32& msg)
{
  motor4.spin(msg.data);
  nh.loginfo("Motor4 data received");
  // nh.loginfo("Motor4 data: %d", msg.data);
}
