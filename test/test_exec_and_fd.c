/*************************************************************************
    > File Name: test_exec_and_fd.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月10日 星期一 21时50分46秒
 ************************************************************************/

#include<unistd.h>
#include<fcntl.h>
#include<iostream>
using namespace std;

int main()
{
//	O_RDONLY O_WRONLY O_RDWR O_CREAT O_APPEND
	int fd = open("exec_hello.c",O_RDWR);
	cout << fd << endl;

	//对打开文件的处理与每个描述符的exec关闭标志值有关，进程中每个文件描述符有一个
	//exec关闭标志(FD_CLOEXEC),若此标志设置，则在执行exec时关闭该描述符，否则该
	//描述符仍然打开。
	//
	//除非特地用fcntl设置了该标志，否则系统的默认操作是在exec后仍保持之中描述符打开，
	//利用这点可以实现I/O重定向
	execl("exec_and_fd","exec_and_fd",NULL);	
	cout << "我不应该被打印出来" << endl;
	
	return 0;
}

















