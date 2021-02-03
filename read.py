# -*- coding: utf-8 -*-

'''

读取测试

'''

import rospy
import time 

from ros_proto.LaserScan_pb2 import LaserScan

scan = LaserScan()

# 测试读取10次要花多少时间

t1 = time.time()

n = 10
for i in range(n):
    f = open("/home/xxx/bag/split/buf_1", "rb")
    scan.ParseFromString(f.read())
    f.close()

t2 = time.time() 

print('time:', (t2-t1)/n )

print('header:', scan.header)
print('angle_min:', scan.angle_min)
print('angle_max:', scan.angle_max)
print('angle_increment:', scan.angle_increment)
print('time_increment:', scan.time_increment)
print('scan_time:', scan.scan_time)
print('range_min:', scan.range_min)
print('range_max:', scan.range_max)

# print('ranges:', scan.ranges)
# print('intensities:', scan.intensities)
