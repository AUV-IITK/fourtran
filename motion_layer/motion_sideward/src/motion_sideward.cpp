#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>
#include <actionlib/server/simple_action_server.h>
#include <motion_common/forwardAction.h>
#include <string>

std_msgs::Int32 pwm;
float buoy_pos_curr = 0;
float buoy_pos_prev = 0;
// a flag to control the start of the server untill data arrives
bool data_received = false;
