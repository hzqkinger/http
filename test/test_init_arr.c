/*************************************************************************
    > File Name: test_init_arr.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月10日 星期一 13时57分01秒
 ************************************************************************/

#include<iostream>
#include<string.h>
//void *memset(void*s,int c,size_t size)
int main()
{
//	int arr[100] = {0};//全都是0
	//int arr[100] = {1};//只有第一个是0，其他全是0
	int arr[100];//全都是随机值
	
	memset(arr,0,sizeof(arr));
	for(int i = 0; i < sizeof(arr)/sizeof(int);++i)
		std::cout << arr[i] << " ";
	std::cout << std::endl;

	return 0;
}
