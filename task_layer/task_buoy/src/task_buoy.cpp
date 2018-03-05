#include <ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>

#include "../include/PID.h"


PID pid_forward(min_val, max_val, kp, ki, kd);
PID pid_sideward(min_val, max_val, kp, ki, kd);

float buoy_pos_curr[3] = {0, 0, 0};
float buoy_pos_prev[3] = {0, 0, 0};
float buoy_pos_final;
// a flag to control the start of the server untill data arrives
bool data_received = false;


void callback_pid(motion_forward::pidConfig &config, uint32_t level)
{
  ROS_INFO("Set Kp:%f Ki:%f Kd:%f", config.kp, config.ki, config.kd);
  object.updateConstants(config.kp, config.ki, config.kd;
}

void ip_sub_callback(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  if(!data_received)
  {
    buoy_pos_curr[0] = msg->data[0];
    buoy_pos_curr[1] = msg->data[1];
    buoy_pos_curr[2] = msg->data[2];
    // change initial data_received flag to true
    data_received = true;
  }
  else
  {
    buoy_pos_prev[0] = buoy_pos_curr[0];
    buoy_pos_prev[1] = buoy_pos_curr[1];
    buoy_pos_prev[2] = buoy_pos_curr[2];

    buoy_pos_curr[0] = msg->data[0];
    buoy_pos_curr[1] = msg->data[1];
    buoy_pos_curr[2] = msg->data[2];  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "task_buoy");
  ros::NodeHandle nh;

  ros::Publisher motor1_pwm_pub = nh.advertise<std_msgs::Int32>("pwm/motor1", 1000);
  ros::Publisher motor2_pwm_pub = nh.advertise<std_msgs::Int32>("pwm/motor2", 1000);
  ros::Publisher motor3_pwm_pub = nh.advertise<std_msgs::Int32>("pwm/motor3", 1000);
  ros::Publisher motor4_pwm_pub = nh.advertise<std_msgs::Int32>("pwm/motor4", 1000);

  ros::Subscriber ip_sub = nh.subscribe<std_msgs::Float32MultiArray>("/fourtran/vision/bouy_coordinates", 1000, ip_sub_callback);

  dynamic_reconfigure::Server<task_buoy::pidConfig> dynamic_server;
  dynamic_reconfigure::Server<task_buoy::pidConfig>::CallbackType cb;

  cb = boost::bind(&callback_pid, _1, _2);
  dynamic_server.setCallback(cb);

  pid_forward.compute(  );
  


}
