/*************************************************************************
    > File Name: test_atoi.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月07日 星期五 23时40分39秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>//atoi
#include<string.h>//strlen
int main()
{
	char line[1024] = "o: 12345\n";
	for(int i = 0; i < strlen(line);++i)
	{
		int num = atoi(line + i);
		printf("%d\n",num);
	}
	return 0;
}
