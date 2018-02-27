#include <ros/ros.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Int32.h>

#include <string>

#include <actionlib/server/simple_action_server.h>
#include <motion_common/forwardAction.h>

#include <dynamic_reconfigure/server.h>
#include <motion_forward/pidConfig.h>

std_msgs::Int32 pwm;
float buoy_pos_curr = 0;
float buoy_pos_prev = 0;
float buoy_pos_final;
// a flag to control the start of the server untill data arrives
bool data_received = false;

class ActionClass
{
protected:
  ros::NodeHandle nh_;
  actionlib::SimpleActionServer<motion_common::forwardAction> forward_server_;
  std::string action_name_;
  motion_common::forwardFeedback feedback_;
  motion_common::forwardResult result_;
  ros::Publisher PWM_arduino;
  float kp, kd, ki, tolerance;

public:
explicit ActionClass(std::string name):
  // initialize the server and call analysisCB in constructor
  forward_server_(nh_, name, boost::bind(&ActionClass::analysisCB, this, _1), false), action_name_(name)
{
  // register preempt callback
  forward_server_.registerPreemptCallback(boost::bind(&ActionClass::preemptCB, this));
  // initialize the PWM_arduino publisher
  PWM_arduino = nh_.advertise<std_msgs::Int32>("/pwm/forward", 1000);
  // start the action server
  forward_server_.start();
}

~ActionClass(void){}

void preemptCB(void)
{
  pwm.data = 0;
  PWM_arduino.publish(pwm);
  ROS_INFO("%s: Preempted", action_name_.c_str());
  forward_server_.setPreempted();
}

int map_to_pwm(float output)
{
  float max_output = 100;
  float min_output = -100;
  if(output > max_output)
    output = max_output;
  if(output < min_output)
    output = min_output;
  float temp = output * 255 / max_output;
  int pwm = static_cast<int>(temp);
  if(pwm > 255)
    pwm = 255;
  if(pwm < -255)
    pwm = -255;
  return pwm;
}

void analysisCB(motion_common::forwardGoal goal_)
{
  int loop_rate = 10;
  ros::Rate loop(loop_rate);

  if (!forward_server_.isActive())
     return;

  while(!data_received)
  {
    ROS_INFO("%s Waiting for camera input", ros::this_node::getName().c_str());
    loop.sleep();
  }

  buoy_pos_final = goal_.goal;
  bool reached = false;
  float error_x, derivative_x = 0, integral_x = 0;
  int count = 0;

  while(!forward_server_.isPreemptRequested() && ros::ok() && count < goal_.checkif_reached)
  {
    float dt = 1.0 / loop_rate;
    error_x = buoy_pos_curr - buoy_pos_final;
    integral_x += error_x * dt;
    derivative_x = (buoy_pos_curr - buoy_pos_prev) / dt;
    float output = kp * error_x + ki * integral_x + kd * derivative_x;
    pwm.data = map_to_pwm(output);

    // check if destination is reached and doesnot overshoot
    if(pwm.data <= tolerance && pwm.data >= -tolerance)
    {
      reached = true;
      pwm.data = 0;
      PWM_arduino.publish(pwm);
      forward_server_.setPreempted();
      count++;
    }
    else
    {
      reached = false;
      count = 0;
    }

    if(forward_server_.isPreemptRequested() || !ros::ok())
    {
      ROS_INFO("%s: Preempted", action_name_.c_str());
      forward_server_.setPreempted();
      reached = false;
      break;
    }

    feedback_.distance_remaining = error_x;
    forward_server_.publishFeedback(feedback_);
    PWM_arduino.publish(pwm);
    ROS_INFO("%s send %d pwm to arduino forward", ros::this_node::getName().c_str(), pwm.data);

    ros::spinOnce();
    loop.sleep();
  }
}

void set_pid(float kp_new, float ki_new, float kd_new, float tolerance_new)
{
  kp = kp_new;
  ki = ki_new;
  kd = kd_new;
  tolerance = tolerance_new;
}
};

// callback function for dynamic server
void callback_pid(motion_forward::pidConfig &config, uint32_t level, ActionClass* object)
{
  // use the set_pid method of ActionClass
  ROS_INFO("Set Kp:%f Ki:%f Kd:%f Tolerance:%f", config.kp, config.ki, config.kd, config.tolerance);
  object->set_pid(config.kp, config.ki, config.kd, config.tolerance);
}

void callback_ip(const std_msgs::Float32MultiArray& msg)
{
  if(!data_received)
  {

    buoy_pos_curr = msg.data[0];
    // change initial data_received flag to true
    data_received = true;
  }
  else
  {
    buoy_pos_prev = buoy_pos_curr;
    float buoy_pos_curr = msg.data[0];
  }
}

int main(int argc, char ** argv)
{
  ros::init(argc, argv, "motion_forward");
  ros::NodeHandle n;
  // subscribe to the bouy_coordinates topics
  ros::Subscriber ip = n.subscribe<std_msgs::Float32MultiArray>("/fourtran/vision/bouy_coordinates", 1000, callback_ip);

  ROS_INFO("%s is waiting for goal: ", ros::this_node::getName().c_str());
  ActionClass object(ros::this_node::getName());

  // dynamic server containing parameters
  dynamic_reconfigure::Server<motion_forward::pidConfig> dynamic_server;
  dynamic_reconfigure::Server<motion_forward::pidConfig>::CallbackType cb;

  cb = boost::bind(&callback_pid, _1, _2, _3);
  dynamic_server.setCallback(cb);

  ros::spin();
  return 0;
}
