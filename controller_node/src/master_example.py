#!/usr/bin/python

import serial
import rospy
from std_msgs.msg import Float32
from roslib.message import get_message_class
from groundbot_msgs.msg import motor

pwm_str=""

def callback(msg):
    global pwm_str
    pwm_str = str(msg.motorEast) + ',' + str(msg.motorWest) + '\n'
    
if __name__ == '__main__':
    rospy.init_node('controller_node')

    rospy.Subscriber('/pwm',motor,callback)

    motor_speed = Float32()

    try:
        serial_connection = serial.Serial("/dev/ttyACM0", 9600, timeout=5)
        rospy.sleep(2)
        
    except:
        rospy.logerr("Could not start serial_connection")
        exit()

    while not rospy.is_shutdown():
        try:
            rospy.loginfo('Inside the spin loop')
            serial_connection.flush()
            pwm_msg = pwm_str.encode('utf-8')
            serial_connection.write(pwm_msg)
            rospy.sleep(0.05)
        except:
            rospy.logerr('Some error has occured')

    rospy.spin()

    serial_connection.close()
