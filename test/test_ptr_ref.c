/*************************************************************************
    > File Name: test_ptr_ref.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月14日 星期五 15时48分44秒
 ************************************************************************/

#include<iostream>
using namespace std;

//void fun(int &*ptr, int &b)错误
void fun(int *&ptr, int &b)
{
	cout << "in fun "<< *ptr << endl;
	ptr = &b;
	cout << "in fun "<< *ptr << endl;
}
int main()
{
	int a = 10,b = 20;
	int *p = &a;
	cout << *p << endl;
	fun(p,b);
	cout << *p << endl;
	return 0;
}
