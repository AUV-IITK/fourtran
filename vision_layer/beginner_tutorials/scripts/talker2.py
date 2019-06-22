#!/usr/bin/env python

import rospy
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10)
    rospy.init_node('talker', anonymous=True)
#    rate = rospy.Rate(10) # 10hz

    n=int(raw_input('How many times I have to DO THIS?? '))
    for x in range(0,n):
    	let = raw_input('Devil %d  ' %x)
        print(let)
        pub.publish(let)

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass
