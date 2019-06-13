/*************************************************************************
    > File Name: test_Content_Length.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月11日 星期二 19时28分52秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	const char *content = "Content-Length: 23456\n";
	const char *tmp ="Content-Length: ";
	printf("tmp len:\t%d\n",strlen(tmp));

	printf("num:\t%d\n",atoi(content + strlen(tmp)));
	
	return 0;
}
