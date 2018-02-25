#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"

void chatterCallback(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  ROS_INFO("I heard: [%s]", msg->data);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "test1");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/fourtran/vision/buoy_coordinates", 1000, chatterCallback);
  ros::spin();
  return 0;
}
