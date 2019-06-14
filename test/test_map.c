/*************************************************************************
    > File Name: test_map.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月14日 星期五 22时36分32秒
 ************************************************************************/

#include<iostream>
using namespace std;
#include<map>
#include<string>

class AA
{
private:
public:
	static map<int,string> mp;
	static void fun()
	{
		int num = 100;
		string s = "abc";
		mp[num] = s;
	}
};

map<int,string> AA::mp;
//static map<int,string> AA::mp;错误

int main()
{
	AA::fun();
	map<int,char> cc;
	cc[1] = 'a';

	return 0;
}
