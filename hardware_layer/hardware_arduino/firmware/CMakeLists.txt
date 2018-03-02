cmake_minimum_required(VERSION 2.8.3)

include_directories(${ROS_LIB_DIR})

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS "-std=gnu++11 ${CMAKE_CXX_FLAGS}")

# Remove this if using an Arduino without native USB (eg, other than Leonardo)
add_definitions(-DUSB_CON)

generate_arduino_firmware(fourtran_test
  SRCS fourtran_test.cpp ${ROS_LIB_DIR}/time.cpp ../src/Motor.cpp
  HDRS ../include/Motor.h ../include/FourtranConfig.h
  BOARD uno
  PORT /dev/ttyACM0
)
