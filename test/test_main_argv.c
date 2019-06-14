/*************************************************************************
    > File Name: test_main_argv.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月14日 星期五 23时01分32秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

// ./a.out 192.168.31.121 8080
int main(int argc,char *argv[])
{
	printf("%s\n",argv[1]);
	printf("%d\n",atoi(argv[2]));

	return 0;
}
