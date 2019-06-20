/*************************************************************************
    > File Name: test_log.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月19日 星期三 22时16分13秒
 ************************************************************************/

#include<stdio.h>
#include<iostream>
using namespace std;

#include"../Log.h"

int main()
{
	LOG("main");
	cout << "11111；" << __FILE__ << __LINE__ << endl;
	cout << "22222: " << __FILE__ << __LINE__ << endl;
	cout << "33333: " << __FILE__ << __LINE__ << endl;
	return 0;
}
