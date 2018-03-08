# buoy_processing

## Overview

This is the ROS package that takes input from the hardware_camera layer, uses [OpenCV](https://opencv.org/) libraries to process the input image, detect a buoy and calculate the distance to it in terms of x, y and z with respect to the camera.

## Setting up buoy_processing

### Dependencies

- [Robot Operating System (ROS)](http://wiki.ros.org) (middleware for robotics),
- Following ROS Packages: [cv_bridge](http://wiki.ros.org/cv_bridge), [image_transport](wiki.ros.org/image_transport), [sensor_msgs](http://wiki.ros.org/sensor_msgs), [geometry_msgs](http://wiki.ros.org/geometry_msgs)

### Customization

The thresholding params in the `buoy_processing.launch` file are currently set to detect a red buoy. You can input your own values to detect a different coloured buoy.

### Building buoy_processing

Run the following code from your catkin workspace:
```
catkin_make --pkg buoy_processing
```

## Usage

To run the buoy_processing node, run:
```
roslaunch buoy_processing buoy_processing.roslaunch
```

## Nodes

### buoy_processing

#### Subscribed Topics

- `hardware_camera/camera/image_raw`

#### Published Topics

- `buoy_processing/buoy_coordinates`

## Bugs & Feature Requests

Please report bugs and request features using the [Issue Tracker](https://github.com/AUV-IITK/fourtran/issues).
