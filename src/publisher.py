#!/usr/bin/env python

import rospy
import cv2
import time
import requests
from std_msgs.msg import String

def getImage():
	cam = cv2.VideoCapture(0)
	# cv2.namedWindow("test")
    ret, frame = cam.read()
    # cv2.imshow("test", frame)
    if not ret:
    	rospy.loginfo("Unable to get image")
	cam.release()
	# cv2.destroyAllWindows()
	return frame

def process(frame):
	url = "http://35.231.86.164/send_pic"
	encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]
	result, encimg = cv2.imencode('.jpg', frame, encode_param)
	if not result:
		rospy.loginfo("Unable to convert image to jpg")
	files = {'file': encimg}
	data = {'cls': 'dog'}

	r = requests.post(url, files=files, data=data)
	return float(r.text)


def talker():
    pub = rospy.Publisher('image_proc', Float, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(1) # 1hz
    while not rospy.is_shutdown():
    	frame = getImage()
		response = process(frame)
        pub.publish(response)
        rate.sleep()

if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass