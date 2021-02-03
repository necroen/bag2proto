# -*- coding: utf-8 -*-

'''

从 ros 包中读取数据，分解到 文件夹 中

每个文件名用什么？ 这里先暂时用 seq 来命名

sudo rm -rf ./*

'''

import rosbag
import rospy

from ros_proto.LaserScan_pb2 import LaserScan

if __name__ == '__main__':
    bag = rosbag.Bag('/home/xxx/bag/a1.bag')

    kk = 0
    # for topic, msg, t in bag.read_messages(topics='/scan' ):
    for topic, msg, t in bag.read_messages():
        
        if topic == '/scan':
            header = msg.header

            # print(header)

            laser_scan = LaserScan() 

            laser_scan.header.seq = header.seq
            laser_scan.header.frame_id = header.frame_id

            laser_scan.header.stamp.sec = header.stamp.secs
            laser_scan.header.stamp.nsec = header.stamp.nsecs

            laser_scan.angle_min = msg.angle_min
            laser_scan.angle_max = msg.angle_max
            laser_scan.angle_increment = msg.angle_increment
            laser_scan.time_increment = msg.time_increment

            laser_scan.scan_time = msg.scan_time
            laser_scan.range_min = msg.range_min
            laser_scan.range_max = msg.range_max

            # print( msg.ranges[0], type(msg.ranges), type(msg.intensities) ) # tuple 类型

            n1 = len(msg.ranges)
            for i in range(n1):
                laser_scan.ranges.append(msg.ranges[i])

            n2 = len(msg.intensities)
            for i in range(n2):
                laser_scan.intensities.append(msg.intensities[i])

            ## laser_scan buf 写入文件夹
            f = open("/home/xxx/bag/split/buf_" + str(laser_scan.header.seq), "w")
            f.write(laser_scan.SerializeToString())
            f.close()

            print('===================== ', kk)
            kk = kk+1