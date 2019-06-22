#!/usr/bin/env python
import cv2
import numpy as np
import rospy
from std_msgs.msg import String

def talker():
    pub = rospy.Publisher('chatter', String, queue_size=10 )
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
	cap=cv2.VideoCapture(0)
        while True:
		_,frame=cap.read()
	       	bl = cv2.blur(frame, (5,5))
		blur = cv2.bilateralFilter(bl,9,75,75)
		blur1 = cv2.GaussianBlur(blur,(15,15),0)

		hsv = cv2.cvtColor(blur1, cv2.COLOR_BGR2HSV)

		lower_green = np.array([20,110,90])
		upper_green = np.array([40,240,240])  
		mask = cv2.inRange(hsv, lower_green, upper_green)
		#res = cv2.bitwise_and(img,img, mask= mask)
		#th3 = cv2.adaptiveThreshold(mask,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C,\
		#            cv2.THRESH_BINARY,11,2)
		med = cv2.medianBlur(mask,25)
		kernel = np.ones((5,5),np.uint8)
		op = cv2.morphologyEx(med, cv2.MORPH_OPEN, kernel)
		clo = cv2.morphologyEx(op, cv2.MORPH_CLOSE, kernel)
		#lap = cv2.Laplacian(clo,cv2.CV_64F)
		#imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
		ret,thresh = cv2.threshold(clo,127,255,0)
		image, contours, hierarchy = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)

		l = len(contours)
		if l>0:
			cnt = contours[0]
			M = cv2.moments(cnt)
			cx = int(M['m10']/M['m00'])
			cy = int(M['m01']/M['m00'])
		       # print(cx)
		       # print(cy)
			area = cv2.contourArea(cnt)
			perimeter = cv2.arcLength(cnt,True)
			epsilon = 0.1*cv2.arcLength(cnt,True)
			approx = cv2.approxPolyDP(cnt,epsilon,True)
			x,y,w,h = cv2.boundingRect(cnt)
			img2 = cv2.rectangle(frame,(x,y),(x+w,y+h),(0,255,0),2)
			#img1 = cv2.drawContours(img, contours, -1, (0,255,0), 3)
			cv2.imshow('mask',img2)
			pub.publish(str(cx))
		else:
			cv2.imshow('mask',frame)
		k = cv2.waitKey(5) & 0xFF
		if k == 27:
		    break
	cap.release()
	cv2.destroyAllWindows()
    rate.sleep()
	
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass

