#include <ros/ros.h>

#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <motion_common/forwardAction.h>

void feedback_cb(motion_common::forwardActionFeedback msg)
{
  ROS_INFO("Remaining distance: %f", msg.feedback.distance_remaining);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "motion_forward_client");
  ros::NodeHandle n;
  // create the action client
  actionlib::SimpleActionClient<motion_common::forwardAction> client("motion_forward", true);
  // subscribe to the feedback
  ros::subscriber feedback_sub = n.subscribe<motion_common::forwardActionFeedback>("motion_forward/feedback", 1000, feedback_cb);
  // wait for server to start
  ROS_INFO("Waiting for server to start. ");
  client.waitForServer();

  ROS_INFO("Action server started, sending goal");
  motion_common::forwardGoal goal;
  //set goal
  goal.goal = 0;
  goal.checkif_reached = 200;
  // send goal to server
  client.sendGoal(goal);

  ros::spin();
  return 0;
}
