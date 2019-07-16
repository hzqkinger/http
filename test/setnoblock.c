/*************************************************************************
    > File Name: setnoblock.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年07月09日 星期二 09时36分57秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<fcntl.h>
void setnonblocking(int fd)
{
	fcntl(fd,GETFL)
}

int main()
{
	setnonblocking(0);
	std::string s;
	std::cin >> s;
	return 0;
}
