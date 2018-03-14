# hardware_arduino

## Overview

This is a ROS package for interfacting arduino through the [rosserial_arduino](http://wiki.ros.org/rosserial_arduino) package. The package is meant to actuate motors connected to the arduino through a motor driver, and publish the measurements taken from various sensors connected to the arduino onto separate topics.

The `hardware_arduino` package has been tested under [ROS](http://www.ros.org) Kinetic and Ubuntu 16.04. The source code is released under a [BSD 3-Clause license](LICENSE.md).

The hardware used are as follows:
* [Arduino UNO](https://store.arduino.cc/usa/arduino-uno-rev3)
* [8V-28V, 5Amp Dual DC Motor Driver with Current Sense](http://www.nex-robotics.com/products/motor-drivers/8v-28v-5amp-dual-dc-motor-driver-with-current-sense.html)
* [100 RPM Side Shaft Heavy Duty DC Gear Motor](http://www.nex-robotics.com/products/motors-and-accessories/100-rpm-side-shaft-heavy-duty-dc-gear-motor.html)


## Setting up Arduino

### Dependencies

- [Robot Operating System (ROS)](http://wiki.ros.org) (middleware for robotics),
- Following ROS Packages: [rosserial](http://wiki.ros.org/rosserial), [rosserial_arduino](http://wiki.ros.org/rosserial_arduino)

### Preparing the Serial Port
Arduino will likely connect to computer as port `/dev/ttyACM# `or `/dev/ttyUSB#` where `#` is a number like 0, 1, 2, etc., depending on how many other devices are connected. The easiest way to make the determination is to unplug all other USB devices, plug in your Arduino, then run the command:
```
ls /dev/ttyACM*
```

Next make sure you have read/write access to the port. Assuming your Arduino is connected on `/dev/ttyACM0`, run the command:

```
ls -l /dev/ttyACM0
```
You should see an output similar to the following:
```
crw-rw---- 1 root dialout 166, 0 2018-03-05 08:31 /dev/ttyACM0
```

Note that only root and the "dialout" group have read/write access. Therefore, you need to be a member of the dialout group. You only have to do this once and it should then work for all USB devices you plug in later on.

To add yourself to the dialout group, run the command:
```
sudo usermod -a -G dialout your_user_name
```
where `your_user_name` is your Linux login name. You will likely have to log out of your X-window session then log in again, or simply reboot your machine if you want to be sure.

When you log back in again, try the command:
```
groups
```
and you should see a list of groups you belong to including dialout.

### Building Arduino code

Run the following command:
```
cd ~/catkin_ws
catkin_make --pkg hardware_arduino
```

### Uploading code to Arduino
Run the following command:
```
cd ~/catkin_ws
source devel/setup.zsh
catkin_make hardware_arduino_firmware_fourtran_test     
catkin_make hardware_arduino_firmware_fourtran_test-upload     
```

## Usage

To connect to the arduino, run:
```
roslaunch hardware_arduino arduino_connect.launch
```

## Nodes

### fourtran_test

Subscribes to topics with PWM data and actuate the motors with that duty cycle.

#### Subscribed Topics

* **`/pwm/motor1`** ([std_msgs/Int32])
* **`/pwm/motor2`** ([std_msgs/Int32])
* **`/pwm/motor3`** ([std_msgs/Int32])
* **`/pwm/motor4`** ([std_msgs/Int32])

## Bugs & Feature Requests

Please report bugs and request features using the [Issue Tracker](https://github.com/AUV-IITK/fourtran/issues).

[std_msgs/Int32]: http://docs.ros.org/api/std_msgs/html/msg/Int32.html
