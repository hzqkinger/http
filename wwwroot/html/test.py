#! /bin/python
#! coding utf-8

#先导入所需的包
import os

#指定路径
path = '/home/hzq/project/http/wwwroot/html'


for root, dirs, files in os.walk(path):
    for name in files:
        if name.endswith(".png"):
            os.remove(os.path.join(root, name))
            print("Delete File: " + os.path.join(root, name))
