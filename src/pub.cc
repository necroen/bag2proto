/*

在 read.cc 的基础上 将 laserscan pub 出去

最好比较一下 pub 出去的 scan 和原始的 scan 是否一致

*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm> // sort

#include <sys/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>

#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

#include "ros_proto/LaserScan.pb.h"

using namespace std;
using namespace ros_proto;

bool func(string a, string b)
{
    int a1 = atoi(a.substr(4, 10).c_str()); // 获取 buf_ 后的数字，并转换成int，然后比较大小
    int b1 = atoi(b.substr(4, 10).c_str());

    return a1 < b1;
}

vector<string> getFiles(string cate_dir)
{
    // function: 获取 cate_dir 目录下的所有文件名
    // param: cate_dir - string类型
    // result：vector<string>类型

    vector<string> files; //存放文件名

    DIR *dir;
    struct dirent *ptr;
    char base[1000];

    if ((dir = opendir(cate_dir.c_str())) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) ///current dir OR parrent dir
            continue;
        else if (ptr->d_type == 8) ///file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            files.push_back(ptr->d_name);
        else if (ptr->d_type == 10) ///link file
            //printf("d_name:%s/%s\n",basePath,ptr->d_name);
            continue;
        else if (ptr->d_type == 4) ///dir
        {
            files.push_back(ptr->d_name);
            /*
		        memset(base,'\0',sizeof(base));
		        strcpy(base,basePath);
		        strcat(base,"/");
		        strcat(base,ptr->d_nSame);
		        readFileList(base);
			*/
        }
    }
    closedir(dir);

    //排序，按从小到大排序
    // sort(files.begin(), files.end());

    sort(files.begin(), files.end(), func); // 加了个 回调函数
    return files;
}

int main(int argc, char *argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ros::init(argc, argv, "bag2proto_pubnode"); //用于解析ROS参数，第三个参数为本节点名
    ros::NodeHandle nh_;                        // 开一个节点
    ros::Publisher scan_pub = nh_.advertise<sensor_msgs::LaserScan>("scan", 1);

    ros::Rate loop_rate(10.0); //定义发布的频率

    string basePath = "/home/xxx/bag/split";

    vector<string> files = getFiles(basePath);

    // for (int i = 0; i < files.size(); i++)
    // {
    //     cout << files[i] << endl;
    // }

    cout << "files num:" << files.size() << endl;

    for (const auto &file : files)
    {
        // cout << file << endl;

        LaserScan scan; // proto 的 LaserScan

        // Read the existing address book.
        fstream input(basePath + "/" + file, ios::in | ios::binary);

        if (!scan.ParseFromIstream(&input))
        {
            cerr << "Failed to parse buf." << endl;
            return -1;
        }

        // cout << "scan ranges:" << scan.ranges(0) << endl;
        // =================================================
        if (1)
        {
            sensor_msgs::LaserScan scan_ros; // ros 的 laserscan

            // protobuf 是用 方法来获取属性?
            // cout << "scan seq:" << scan.header().seq() << endl;

            scan_ros.header.seq = scan.header().seq();
            ros::Time ts(scan.header().stamp().sec(), scan.header().stamp().nsec());

            scan_ros.header.stamp = ts;

            scan_ros.header.frame_id = scan.header().frame_id();

            scan_ros.angle_min = scan.angle_min();
            scan_ros.angle_max = scan.angle_max();
            scan_ros.angle_increment = scan.angle_increment();
            scan_ros.time_increment = scan.time_increment();
            scan_ros.range_min = scan.range_min();
            scan_ros.range_max = scan.range_max();

            int n = 1800;
            scan_ros.ranges.resize(n);
            scan_ros.intensities.resize(n);

            // for (unsigned int i = 0; i < n; i++)
            // {
            //     cout << "i:" << i << scan.ranges(i) << scan.intensities(i) << endl;
            //     // scan_ros.ranges[i] = scan.ranges(i);
            //     // scan_ros.intensities[i] = scan.intensities(i);
            // }

            for (const auto &v : scan.ranges())
            {
                scan_ros.ranges.push_back(v);
            }

            // 有的时候 intensities 是空的
            for (const auto &v : scan.intensities())
            {
                scan_ros.intensities.push_back(v);
            }

            scan_pub.publish(scan_ros); // 这里也可以不pub，用作其他用途，比如接入某个算法做处理
            loop_rate.sleep();
        }
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}