/*************************************************************************
    > File Name: exec_and_fd.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月10日 星期一 22时00分42秒
 ************************************************************************/
#include<iostream>
 using namespace std;
#include<fcntl.h>

int main()
{
	int fd = open("exec_hello.c",O_RDWR);
	cout << fd << endl;
	return 0;
}
