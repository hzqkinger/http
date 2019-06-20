#ifndef _LOG_H
#define _LOG_H
/*************************************************************************
    > File Name: Log.h
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月19日 星期三 22时12分44秒
 ************************************************************************/

#include<iostream>

void Log(std::string msg,std::string file,int line)
{
	std::cout << file << ":" << line << " : " << msg << std::endl;
}
#define LOG(msg) Log(msg,__FILE__,__LINE__)

#endif
