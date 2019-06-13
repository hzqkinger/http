/*************************************************************************
    > File Name: test_setenv.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月11日 星期二 17时57分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main()
{
	setenv("AAA","hzq",0);
	char buff[20] = {0};
	sprintf(buff,getenv("AAA"));
	//char *sp = getenv("AAA");
	printf("%s\n",buff);

	return 0;
}
