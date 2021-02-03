### <center> bag2proto</center>
The purpose is to facilitate single-step debugging.  
1, write.py: decompose rosbag into protobuf format files.  
2, pub.cc: read protobuf cache and publish to ros.  
 
Only the LaserScan format is implemented now.  

这个 ros 包完成的功能：  

在 python 中读取 ros 包，将每条 message 分解成 protobuf 形式的 buf 到文件夹中。  
然后在 c++ 中 读取这些 buf，转换成对应的ros格式。 

目的是分解ros包，持久化储存，方便单步调试。   

使用方法：  
1, 先去执行 ros_proto/autogen.py，自动生成 proto 的 python 和 c++ 文件，要修改路径为proto文件夹的当前路径。
2, 执行 write.py，对ros包做分解，分解结果保存到split文件夹。  
3, 执行 src/pub.cc 生成的可执行文件 bag2proto_pubnode，读取 buf 并 pub 出去。  

protobuf要事先安装好，write.py 的执行需要事先
```sh
sudo pip install protobuf
```

2021.02.03 16:44   
这里先只实现了 LaserScan 格式的数据   