#########################################################################
# File Name: run.sh
# Author: 黄志强
# mail: 2069085311@qq.com
#########################################################################
#!/bin/bash

make clean
make

./main 192.168.32.128 8080
#./main 192.168.122.2 8080

# 连宿舍的wifi，执行下面语句
# ./main 192.168.31.121 8080
# gdb ./main

# 连图书馆的wifi时，执行下面语句
# ./main 10.210.25.17 8080


# windows上的浏览器访问不了服务器的原因：
# 一、改变虚拟机的网络适配器，
# 二、关闭防火墙：（su   systemctl firewalld.service;systemct disable firewalld.service）


# 使用git进行版本控制（注意，在root下进行提交）
# 一、git add *
# 二、git commit -m "xxxxxx"
# 三、git push
# 四、git reflog
# 五、git reset --hard xxxx


