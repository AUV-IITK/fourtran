# Setting up Arduino

## Preparing the Serial Port
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
crw-rw---- 1 root dialout 166, 0 2013-02-24 08:31 /dev/ttyACM0
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

## Building Arduino code

Run the following command:
```
cd ~/catkin_ws
catkin_make --pkg hardware_arduino
```

## Uploading code to Arduino
Run the following command:
```
cd ~/catkin_ws
source devel/setup.zsh
catkin_make hardware_arduino_firmware_fourtran_test     
catkin_make hardware_arduino_firmware_fourtran_test-upload     
```

## Running the Arduino node
Run the following command:
```
roscore
rosrun rosserial_python serial_node.py /dev/ttyACM0   
```
