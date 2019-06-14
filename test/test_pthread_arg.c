/*************************************************************************
    > File Name: test_pthread_arg.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月03日 星期一 21时57分18秒
 ************************************************************************/

#include<stdio.h>
#include<pthread.h>
class A{
public:
	static void *run(void* arg)
	{
		int a = *(int*)arg;
		printf("%d\n",a);
		printf("--------------------------\n");
	}
};
int main()
{
	pthread_t tid;
	int a = 10;
	pthread_create(&tid,NULL,A::run,(void*)&a);
	//在这里测试时，最好设置线程属性为可结合的,否则主线程会先结束掉。
	pthread_join(tid,NULL);
	return 0;
}
