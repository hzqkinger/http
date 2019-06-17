/*************************************************************************
    > File Name: test_c++.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月17日 星期一 17时24分58秒
 ************************************************************************/

#include"testCplus.h"

void AA::print()
{
	cout << "a = " << a << endl;
}
void AA::add(int num)
{
	a += num;
	print();
}

//定义静态成员时，不能出现static关键字
void AA::sub(int num)
{
	b -= num;
}
int AA::b = 20;
