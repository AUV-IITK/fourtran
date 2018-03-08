#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"
#include "sensor_msgs/Image.h"
#include "opencv2/imgproc/imgproc.hpp"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <geometry_msgs/PointStamped.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

ros::Publisher coordinates_pub;
float focal_length;
float known_width;
int low_bgr[3];
int high_bgr[3];
std::string camera_frame = "camera_link";

void imageCallback(const sensor_msgs::Image::ConstPtr& msg)
{
  cv_bridge::CvImagePtr cv_img_ptr;
  try {
    cv_img_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv::Mat src = cv_img_ptr->image;
    if(!src.empty()) {
      //// pre-processing image
      // applying gaussian blur for smoothening
      cv::blur(src, src, cv::Size(3,3));
      // thresholding image
      cv::Mat thresholded;
      cv::inRange(src, cv::Scalar(low_bgr[0], low_bgr[1], low_bgr[2]), cv::Scalar(high_bgr[0], high_bgr[1], high_bgr[2]), thresholded);
      // identifying contour
      std::vector<std::vector<cv::Point> > contours;
      std::vector<cv::Vec4i> hierarchy;
      //// finding contour of largest area
      cv::findContours(thresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
      ROS_INFO("contours size = %d", contours.size());
      if(contours.size() != 0) {
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
        ROS_INFO("Here9");
        ROS_INFO("index = %d", index);
        mu[0] = moments(contours[index], false);
        ROS_INFO("Here10");
        std::vector<cv::Point2f> mc(1);
        ROS_INFO("Here11");
        mc[0] = cv::Point2f( mu[0].m10/mu[0].m00 , mu[0].m01/mu[0].m00 );
        ROS_INFO("Here12");
        cv::Rect bounding_rectangle = cv::boundingRect(cv::Mat(contours[index]));
        ROS_INFO("Here13");
        float x = (bounding_rectangle.br().x + bounding_rectangle.tl().x)/2 - (src.size().width)/2;
        float y = ((float)src.size().height)/2 - (bounding_rectangle.br().y + bounding_rectangle.tl().y)/2;
        float z = (known_width * focal_length) / (bounding_rectangle.br().x + bounding_rectangle.tl().x);
        ROS_INFO("Here14");

        // publish coordinates message
        geometry_msgs::PointStamped buoy_point_message;
        ROS_INFO("Here15");

        buoy_point_message.header.stamp = ros::Time();
        buoy_point_message.header.frame_id = camera_frame.c_str();
        buoy_point_message.point.x = x;
        buoy_point_message.point.y = y;
        buoy_point_message.point.z = z;
        ROS_INFO("Here16");

        coordinates_pub.publish(buoy_point_message);
        ROS_INFO("Here17");
        ROS_INFO("Buoy Location (x, y, z) = (%.2f, %.2f, %.2f)", x, y, z);
      }
    }
  }
  catch(cv_bridge::Exception& e) {
    return;
  }
}

int main(int argc, char **argv)
{
  // initiliazing ROS node
  ros::init(argc, argv, "buoy_processing");
  ros::NodeHandle nh;

  // initializing publishers
  coordinates_pub = nh.advertise<geometry_msgs::PointStamped>("/vision_layer/buoy_processing/buoy_coordinates", 1000);

  //initializing subscribers
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber image_raw_sub = it.subscribe("/hardware_layer/hardware_camera/camera/image_raw", 1, imageCallback);

  ros::param::get("buoy_processing/focal_length", focal_length);
  ros::param::get("buoy_processing/known_width", known_width);
  ros::param::get("buoy_processing/low_b", low_bgr[0]);
  ros::param::get("buoy_processing/low_g", low_bgr[1]);
  ros::param::get("buoy_processing/low_r", low_bgr[2]);
  ros::param::get("buoy_processing/high_b", high_bgr[0]);
  ros::param::get("buoy_processing/high_g", high_bgr[1]);
  ros::param::get("buoy_processing/high_r", high_bgr[2]);
  ROS_INFO("Thresholding BGR with range: (%d, %d, %d) - (%d, %d, %d)", low_bgr[0], low_bgr[1], low_bgr[2], high_bgr[0], high_bgr[1], high_bgr[2]);
  ros::spin();
  return 0;
}
