/*************************************************************************
    > File Name: test.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月02日 星期日 16时07分37秒
 ************************************************************************/
#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<sys/sendfile.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>

#include<stdio.h>
#include<iostream>
using namespace std;
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>

#include<string>
#include<map>

#include"json/json.h"


class httpServer{
private:
	static map<int,string> _status;
public:
	static map<int,string> _request;//静态变量初始化

	static void startup(int &sockfd_ser,const char *ip,short port);
	static void statusInit();//将状态码及其描述放到map表中
	static void echo_header(int sock,int status);
	static void clear_header(int sock);
	static int echo_www(int sock, const char *path, int size);
	static int get_line(int sock, char line[], int num);// line -> \r,\r\n,\n->\n
	static void print_request(int sock);
	static void exec_cgi(int sockfd_cli,char *method,char *path,char *queryString);

//--------------------------------------------------------------------
	static void requestInit(int sockfd_cli,char *line,char *&method,char *&url,char *&queryString,char *&version);
	static void responeGet(int sockfd_cli,char *path,char *url,const char *homePage);
	static void responsePost(int sockfd_cli,char *method,char *path,char *url,char *&queryString);
	static void *worker(void *arg);

//--------------------------------------------------------------------
	static void getRequestToMap(int sockfd_cli);
	static void responeGet(int sockfd_cli);
	static void responsePost(int sockfd_cli);
};

#endif
