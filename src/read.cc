
/*

依此读取 split 文件夹下所有buf， 并转换成 proto 格式

g++ read2.cpp ./ros_proto/*.pb.cc -o read2 -lprotobuf -std=c++11 -L /usr/local/lib -lpthread

./read2

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

    string basePath = "/home/xxx/bag/split";

    std::string::size_type sz; // alias of size_t

    vector<string> files = getFiles(basePath);

    // for (int i = 0; i < files.size(); i++)
    // {
    //     cout << files[i] << endl;
    // }

    cout << "files num:" << files.size() << endl;

    for (const auto &file : files)
    {
        // cout << file << endl;

        LaserScan scan;

        // Read the existing address book.
        fstream input(basePath + "/" + file, ios::in | ios::binary);

        if (!scan.ParseFromIstream(&input))
        {
            cerr << "Failed to parse buf." << endl;
            return -1;
        }

        cout << "scan ranges:" << scan.ranges(0) << endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}