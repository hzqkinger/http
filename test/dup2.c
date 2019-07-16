/*************************************************************************
    > File Name: dup2.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年07月09日 星期二 10时07分19秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main()
{
	int fd = open("hz.txt",O_RDWR);
	int newfd;
	//dup2(fd,newfd);
	newfd = dup(fd);
	printf("%d\n",newfd);
	char ch;
	for(int i = 0;i < 5;++i){
		read(fd,&ch,1);
		printf("%c ",ch);
		read(newfd,&ch,1);
		printf("%c ",ch);
	}

	return 0;
}
