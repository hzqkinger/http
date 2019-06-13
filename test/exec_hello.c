/*************************************************************************
    > File Name: exec_hello.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月10日 星期一 16时03分44秒
 ************************************************************************/

#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	setenv("AAA","In father reset",1);

	pid_t pid;
	if((pid = fork()) == 0){
		printf("In child:\t%s\n",getenv("AAA"));
	}else{
		printf("In father:\t%s\n",getenv("AAA"));
	}
	printf("hello exec\n");
	//printf("In public:\t%s\n",getenv("AAA"));
	//printf("%s\n",getenv("PATH"));
	wait(NULL);
	return 0;
}
