/*************************************************************************
    > File Name: test_getStatusFromFile.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月14日 星期五 17时09分50秒
 ************************************************************************/

#include<stdio.h>
#include<iostream>
using namespace std;
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

#include<map>
#include<string>
#include<string.h>
#include<stdlib.h>

//第一个getline函数是在string头文件中，定义的是一个全局函数
//istream& getline ( istream& is, string& str, char delim = '\n');
//
//第二getline函数是isream的成员函数
//istream& getline (char* s, streamsize n, char delim = '\n');
int main()
{
	map<int,string> mp;
	char buff[1024] = {0};

	int fd = open("../Status",O_RDONLY);dup2(fd,0);
	while(cin.getline(buff,1024,'#'))
	{
		//printf("%s\n",buff);
		cin.ignore(1024,'\n');

//		char * strtok (char * str, const char * delimiters);
//		int num = atoi(strtok(buff,":"));
//		string s = strtok(NULL,":");
//		//cout << num << " ---> " << s << endl;
//		mp[num] = s;
//		//下面这句，居然出错了
//		mp[atoi(strtok(buff,":"))] = string(strtok(NULL,":"));

		//sleep(1);
	}

//for循环语句，auto关键字,都是只能在c++11中使用

//	for(map<int,string>::iterator it = mp.begin();it != mp.end();++it)
//		cout << it->first << "--->" << it->second << endl;
	return 0;
}
