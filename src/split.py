#!/usr/bin/python

import rospy
from rospy import Subscriber, Publisher
from sensor_msgs.msg import Image, CameraInfo
from cv_bridge import CvBridge
import cv2
import yaml
import numpy as np

rospy.init_node('zed_split')


def cb(msg):
    img = bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
    w2 = img.shape[1]
    w = w2 / 2
    left = img[:, 0:w, :]
    right = img[:, w:w2, :]
    if pub_l:
        pub_l.publish(bridge.cv2_to_imgmsg(left, encoding='bgr8'))
    if pub_r:
        pub_r.publish(bridge.cv2_to_imgmsg(right, encoding='bgr8'))
    left_rect = cv2.remap(left, map_l1, map_l2, cv2.INTER_CUBIC)
    right_rect = cv2.remap(right, map_r1, map_r2, cv2.INTER_CUBIC)
    msg_l = bridge.cv2_to_imgmsg(left_rect, encoding='bgr8')
    msg_r = bridge.cv2_to_imgmsg(right_rect, encoding='bgr8')
    now = msg.header.stamp
    # now = rospy.Time.now()
    # now = rospy.get_rostime()
    msg_l.header.stamp = now
    msg_r.header.stamp = now
    pub_rect_l.publish(msg_l)
    pub_rect_r.publish(msg_r)
    ci_l.header.stamp = now
    ci_r.header.stamp = now
    pub_ci_l.publish(ci_l)
    pub_ci_r.publish(ci_r)


def load_yaml(filename):
    stream = file(filename, 'r')
    f = yaml.load(stream)
    return f


def getMatrix(src, name):
    rows = src[name]['rows']
    cols = src[name]['cols']
    return np.array(src[name]['data']).reshape((rows, cols))


calib_l = load_yaml('/home/yh/caffe/mine_ws/src/ssd_detection/script/left.yaml')
calib_r = load_yaml('/home/yh/caffe/mine_ws/src/ssd_detection/script/right.yaml')

bridge = CvBridge()

camMat1 = getMatrix(calib_l, 'camera_matrix')
dis1 = getMatrix(calib_l, 'distortion_coefficients')
R1 = getMatrix(calib_l, 'rectification_matrix')
P1 = getMatrix(calib_l, 'projection_matrix')
camMat2 = getMatrix(calib_r, 'camera_matrix')
dis2 = getMatrix(calib_r, 'distortion_coefficients')
R2 = getMatrix(calib_r, 'rectification_matrix')
P2 = getMatrix(calib_l, 'projection_matrix')

new_size = (648, 314)
map_l1, map_l2 = cv2.initUndistortRectifyMap(camMat1, dis1, R1, P1, new_size, cv2.CV_32FC1)
map_r1, map_r2 = cv2.initUndistortRectifyMap(camMat2, dis2, R2, P2, new_size, cv2.CV_32FC1)


def fill_camera_info(calib):
    ci = CameraInfo()
    ci.width = int(calib['image_width'])
    ci.height = int(calib['image_height'])
    ci.distortion_model = calib['distortion_model']
    ci.D = calib['distortion_coefficients']['data']
    ci.K = calib['camera_matrix']['data']
    ci.R = calib['rectification_matrix']['data']
    ci.P = calib['projection_matrix']['data']
    return ci

ci_l = fill_camera_info(calib_l)
ci_r = fill_camera_info(calib_r)

raw = True
if raw:
  sub = Subscriber('/camera/image_raw_decompressed', Image, cb)
  pub_rect_l = Publisher('/camera/left/image_raw', Image, queue_size=10)
  pub_rect_r = Publisher('/camera/right/image_raw', Image, queue_size=10)
else:
  sub = Subscriber('/camera/image_raw_decompressed', Image, cb)
  pub_rect_l = Publisher('/camera/left/image_rect_color', Image, queue_size=10)
  pub_rect_r = Publisher('/camera/right/image_rect_color', Image, queue_size=10)
# pub_l = Publisher('/camera/left/image_raw', Image, queue_size=10)
# pub_r = Publisher('/camera/right/image_raw', Image, queue_size=10)
pub_l = None
pub_r = None
pub_ci_l = Publisher('camera/left/camera_info', CameraInfo, queue_size=10)
pub_ci_r = Publisher('camera/right/camera_info', CameraInfo, queue_size=10)


rospy.spin()

