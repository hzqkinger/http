/*************************************************************************
    > File Name: test_c++.h
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月17日 星期一 17时25分15秒
 ************************************************************************/

//#ifndef TESTCPLUS_H_
//#define TESTCPLUS_H_

#include<iostream>
using namespace std;

class AA{
public:
	AA(int num = 10):a(num){}
	void print();
	void add(int num);
	static void sub(int num);
private:
	int a;
	static int b;
};

//#endif
