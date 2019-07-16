/*************************************************************************
    > File Name: fcntl.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年07月09日 星期二 09时53分39秒
 ************************************************************************/

#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<iostream>

int main()
{
	int fd = open("hz.txt",O_RDWR);
	printf("%d\n",fd);
	// int newfd = fcntl(fd,F_DUPFD,7);
	/* int fcntl(int fd,int op,int arg);
	 * fd		需要复制的文件描述符
	 * op		F_DUPFD是复制操作
	 * arg		寻找大于等于arg的可用文件描述符作为返回值
	 * 返回值	返回一个文件描述符
	 * */
	//char ch;
	//for(int i = 0;i < 5;++i){
	//	read(fd,&ch,1);
	//	printf("%c ",ch);
	//	read(newfd,&ch,1);
	//	printf("%c ",ch);
	//}

	int flag = fcntl(0,F_GETFL,0);
	printf("%d\n",flag);
	flag |= O_NONBLOCK;
	char ch;
	std::cin >> ch;
	fcntl(0,F_SETFL,flag);
	/* int fcntl(int fd,int op,int flag);
	 * fd		需要设置的文件描述符
	 * op		需要进行的操作
	 * flag		文件状态标志
	 * 返回值	返回flag
	 * */
	printf("%d\n",flag);
	std::cin >> ch;

	return 0;
}
