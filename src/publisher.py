#!/usr/bin/env python

import rospy
import cv2
import time
import requests
from std_msgs.msg import Float64

def getImage(cam):
    ret, frame = cam.read()
    frame = cv2.resize(frame, (200,200))
    cv2.imwrite("frame.jpg", frame)
    if not ret:
        rospy.loginfo("Unable to get image")
    return frame

def process():
	url = "http://35.231.86.164/send_pic"
	#encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
	#result, encimg = cv2.imencode('.jpg', frame)
        #cv2.imwrite("frame.jpg", frame)
	#if not result:
		#rospy.loginfo("Unable to convert image to jpg")
	files = {'file': open("frame.jpg", "rb")}
	data = {'cls': 'person'}

	r = requests.post(url, files=files, data=data)
	return float(r.text)


def talker(cam):
    pub = rospy.Publisher('image_proc', Float64, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(0.5) # 1hz
    while not rospy.is_shutdown():
    	frame = getImage(cam)
        response = process()
        print(response)
        pub.publish(response)
        rate.sleep()

if __name__ == '__main__':
    try:
        cam = cv2.VideoCapture(1)
        talker(cam)
    except rospy.ROSInterruptException:
        pass
