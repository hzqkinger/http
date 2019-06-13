/*************************************************************************
    > File Name: test_pthread_return.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月11日 星期二 15时50分34秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

void *run1(void *arg)
{
	printf("In thread one\n");
//	return NULL;//return退出，不会影响主线程以及其他线程
	//
	// pthread_cancel是请求同一进程中的一个线程退出。注意，该函数并不等待线程终止，
	// 它仅仅是提出请求。即调用了该函数也不等于目标线程马上就会退出，目标线程有可能
	// 再运行一段时间后到达取消点才退出，甚至有可能不响应退出。
//	pthread_cancel(pthread_self());
//	sleep(1);
	
	pthread_exit(NULL);

	printf("我不会被打印出来\n");
}
void *run2(void *arg)
{
	while(1){
		printf("In thread two\n");
		sleep(1);
	}
}
int main()
{
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,run1,NULL);
	pthread_detach(tid1);
	pthread_create(&tid2,NULL,run2,NULL);
	pthread_detach(tid2);

	for(int i = 1; ;++i){
		printf("%d ",i);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
