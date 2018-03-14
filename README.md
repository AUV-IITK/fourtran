# FourTran: Four-wheeled Ground Robot

This repository contains the code for our ground robot `FourTran`. The robot would be actuated using four brushed DC motors and contains omni-wheels which enable it to perform motions like sway and differential turning.

The main purpose of the robot is to act as a testing robot for our vision and controls algorithms which we later hope to implement on our underwater vehicle.

## How to build the repository?

1. Create a catkin worspace following the guidelines given [here](http://wiki.ros.org/catkin/Tutorials/create_a_workspace)
```bash
mkdir -p ~/catkin_ws/src
cd ~/catkin_ws/src
catkin_init_workspace
cd ..
catkin_make
```

2. Clone this repository to your catkin workspace
```bash
cd ~/catkin_ws/src
git clone https://github.com/AUV-IITK/fourtran.git
```

3. [Install all dependency packages to run the repository](#additional-ros-packages-required)
```bash
# TODO List
```

4. Build the package using [`catkin_make`](http://wiki.ros.org/catkin/commands/catkin_make)
```bash
cd ~/catkin_ws
# To maximize performance, build the workspace in Release mode
catkin_make -DCMAKE_BUILD_TYPE=Release
```

## Contribution Guidelines

To get started with contributing to this repository, look out for open issues [here](https://github.com/AUV-IITK/fourtran/issues). Kindly read the [__Developer's Guide__](https://github.com/AUV-IITK/AUVWiki/wiki/Developers-Guide) before sending a pull request! :)
