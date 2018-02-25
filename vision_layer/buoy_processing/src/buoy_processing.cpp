#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "buoy_processing");
  ros::NodeHandle n;
  ros::Publisher coordinates_pub = n.advertise<std_msgs::Float32MultiArray>("/fourtran/vision/buoy_coordinates", 1000);
  float focal_length = 0.0;
  float known_width = 0.0;
  n.getParam("/fourtran/vision/distance_z/focal_length", focal_length);
  n.getParam("/fourtran/vision/distance_z/known_width", known_width);
  int low_b = 0;
  int low_g = 0;
  int low_r = 0;
  int high_b = 0;
  int high_g = 0;
  int high_r = 0;
  n.getParam("/fourtran/vision/thresholding/low_b", low_b);
  n.getParam("/fourtran/vision/thresholding/low_g", low_g);
  n.getParam("/fourtran/vision/thresholding/low_r", low_r);
  n.getParam("/fourtran/vision/thresholding/high_b", high_b);
  n.getParam("/fourtran/vision/thresholding/high_g", high_g);
  n.getParam("/fourtran/vision/thresholding/high_r", high_r);
  ROS_INFO("(%d, %d, %d) - (%d, %d, %d)", low_b, low_g, low_r, high_b, high_g, high_r);
  VideoCapture cap;
  if(!cap.open(0)) ROS_INFO("Problem with video camera");
  ros::Rate loop_rate(0.5);
  while(true && ros::ok()) {
    Mat src;
    cap >> src;
    if( src.empty() ) break;
    blur(src, src, Size(3,3));
    Mat thresholded;
    inRange(src, Scalar(low_b, low_g, low_r), Scalar(high_b, high_g, high_r), thresholded);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(thresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    int index = -1;
    float max_area = 0.0;
    float area = 0.0;
    for( int i = 0; i< contours.size(); i++ ) {
      area = contourArea(contours[i]);
      if(area>max_area) {
        index = i;
 	      max_area =area;
      }
    }
    vector<Moments> mu(1);
    mu[0] = moments(contours[index], false);
    vector<Point2f> mc(1);
    mc[0] = Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 );
    Rect bounding_rectangle = boundingRect(Mat(contours[index]));
    float x = (bounding_rectangle.br().x + bounding_rectangle.tl().x)/2 - (src.size().width)/2;
    float y = ((float)src.size().height)/2 - (bounding_rectangle.br().y + bounding_rectangle.tl().y)/2;
    float z = (known_width * focal_length) / (bounding_rectangle.br().x + bounding_rectangle.tl().x);
    std_msgs::Float32MultiArray message;
    message.data.clear();
    message.data.push_back(x);
    message.data.push_back(y);
    message.data.push_back(z);
    ROS_INFO("(x, y, z) = (%.2f, %.2f, %.2f)", x, y, z);
    coordinates_pub.publish(message);
    ros::spinOnce();
    loop_rate.sleep();
  }
}
