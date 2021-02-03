# -*- coding: utf-8 -*-

# 自动编译 .proto 后缀的文件

import os
 
def getFileName(path):
    ''' 获取指定目录下的所有指定后缀的文件名 '''
    
    file_list = []
    
    f_list = os.listdir(path)
    # print f_list
    for i in f_list:
        # os.path.splitext():分离文件名与扩展名
        if os.path.splitext(i)[1] == '.proto':
            # print i
            file_list.append(i)
        
    return file_list
 
 
if __name__ == '__main__':
    path = os.getcwd()
    filelist = getFileName(path)
    
    path1 = path + '/'

    # print(len(filelist))

    # 生成 cpp 文件 
    for x in filelist:
        # print(x)
        cmd_str = 'protoc -I=' + path1 + ' -I=' + path1 + ' --cpp_out='+ path1 + ' ' + path1+x
        os.system(cmd_str)

    # 生成 py 文件 
    for x in filelist:
        cmd_str = 'protoc -I=' + path1 + ' -I=' + path1 + ' --python_out='+ path1 + ' ' + path1+x
        os.system(cmd_str)
