#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main(int argc, char **argv)
{
  // initiliazing ROS node
  ros::init(argc, argv, "buoy_processing");
  ros::NodeHandle n;
  ros::Rate loop_rate(0.5);

  // initializing publishers
  ros::Publisher coordinates_pub = n.advertise<std_msgs::Float32MultiArray>("buoy_coordinates", 1000);
  std_msgs::Float32MultiArray coordinate_message;

  //// reading paramters
  // for depth estimation
  float focal_length = 0.0;
  float known_width = 0.0;
  n.getParam("buoy_distance_z/focal_length", focal_length);
  n.getParam("buoy_distance_z/known_width", known_width);
  // for thresholding
  int low_bgr[] = {0, 0, 0};
  int high_bgr[] = {0, 0, 0};
  n.getParam("buoy_thresholding/low_b", low_bgr[0]);
  n.getParam("buoy_thresholding/low_g", low_bgr[1]);
  n.getParam("buoy_thresholding/low_r", low_bgr[2]);
  n.getParam("buoy_thresholding/high_b", high_bgr[0]);
  n.getParam("buoy_thresholding/high_g", high_bgr[1]);
  n.getParam("buoy_thresholding/high_r", high_bgr[2]);
  ROS_INFO("Thresholding BGR with range: (%d, %d, %d) - (%d, %d, %d)", low_bgr[0], low_bgr[1], low_bgr[2], high_bgr[0], high_bgr[1], high_bgr[2]);

  // caputring frame using OpenCV
  cv::VideoCapture cap;

  if(!cap.open(0))
    ROS_ERROR("Problem with video camera");

  while(true && ros::ok())
  {
    cv::Mat src;
    cap >> src;
    if( src.empty() )
      break;

    //// pre-processing image
    // applying gaussian blur for smoothening
    cv::blur(src, src, cv::Size(3,3));
    // thresholding image
    cv::Mat thresholded;
    cv::inRange(src, cv::Scalar(low_bgr[0], low_bgr[1], low_bgr[2]), cv::Scalar(high_bgr[0], high_bgr[1], high_bgr[2]), thresholded);
    // identifying contour
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(thresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    //// finding contour of largest area
    int index = -1;
    float max_area = 0.0;
    float area = 0.0;
    for( int i = 0; i< contours.size(); i++ )
    {
      area = cv::contourArea(contours[i]);
      if(area > max_area)
      {
        index = i;
 	      max_area =area;
      }
    }
    // caluclating center of mass of contour using moments
    std::vector<cv::Moments> mu(1);
    mu[0] = moments(contours[index], false);
    std::vector<cv::Point2f> mc(1);
    mc[0] = cv::Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 );
    cv::Rect bounding_rectangle = cv::boundingRect(cv::Mat(contours[index]));
    float x = (bounding_rectangle.br().x + bounding_rectangle.tl().x)/2 - (src.size().width)/2;
    float y = ((float)src.size().height)/2 - (bounding_rectangle.br().y + bounding_rectangle.tl().y)/2;
    float z = (known_width * focal_length) / (bounding_rectangle.br().x + bounding_rectangle.tl().x);

    // publish coordinates message
    coordinate_message.data.clear();
    coordinate_message.data.push_back(x);
    coordinate_message.data.push_back(y);
    coordinate_message.data.push_back(z);
    coordinates_pub.publish(coordinate_message);
    ROS_INFO("Buoy Location (x, y, z) = (%.2f, %.2f, %.2f)", x, y, z);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
