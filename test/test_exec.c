/*************************************************************************
    > File Name: test_exec.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月10日 星期一 15时46分10秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main()
{
//---  execl  ----------------------------------------------------------------------
	//execl("/bin/ls","ls -l",NULL);
	//execl("/home/hz/http/test/exec_hello","./exec_hello",NULL);//提供绝对路径
/*
 *提供绝对路径可以找到可执行文件；提供相对路径则在当前目录下寻找该文件，找不到替换失败
 */
	//execl("exec_hello","exec_hello",NULL);//提供相对路径

//---   execlp   ---------------------------------------------------------------------

	//execlp("/home/hz/http/test/exec_hello","exec_hello",NULL);//提供绝对路径
/*
 * 提供绝对路径可值正确找到可执行文件；提供相对路径时，函数先从PATH环境变量提供的
 * 路径下去找有没有相应的可执行文件，有则可正确替换，否则不行
 */
	//execlp("exec_hello","exec_hello",NULL);//提供相对路径

//---   execle   ---------------------------------------------------------------------

//以e结尾的exec函数族，调用之后从此不再继承调用者的环境变量，只用用户提供的
// 这个时候，根本没有PATH这个环境变量了
	//execle("/home/hz/http/test/exec_hello","exec_hello",NULL,NULL);//提供绝对路径
/*
 *提供绝对路径可以找到可执行文件；提供相对路径则在当前目录下寻找该文件，找不到替换失败
 */
//	execle("exec_hello","exec_hello",NULL,NULL);//提供相对路径

	printf("我是不会打印出来的\n");
	return 0;
}
