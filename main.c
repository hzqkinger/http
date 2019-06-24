#include"HttpServer.h"
#include<sys/select.h>

void Usage(const char *str)/*{{{*/
{
	printf("Usage:\t%s ip port\n",str);
}/*}}}*/

// ./main ip port
int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}

	int sockfd_ser;
	//httpServer::startup(sockfd_ser,"192.168.31.121",atoi("8080"));
	httpServer::startup(sockfd_ser,argv[1],atoi(argv[2]));
	httpServer::statusInit();
	
	int connCliNum = 0;//看看服务器总计服务的连接数目

	int fd_list[1024];
	const int fd_list_size = sizeof(fd_list)/sizeof(fd_list[0]);
	memset(fd_list,0xff,sizeof(fd_list));
	fd_list[0] = sockfd_ser;//第零个位置总是放服务器的监听套接字描述符
	int maxfd = sockfd_ser;//寻找最大的文件描述符

	fd_set rdset;//我们只关心文件描述符的读事件
	//---因为我认为文件描述符的读事件就绪了，那么它的写事件也就绪了---
	for(;;)
	{
		//每次进来都需要重新设置关心事件，因为rdset是输入输出型参数
		FD_ZERO(&rdset);
		for(int i = 0;i < fd_list_size;++i)
		{
			if(fd_list[i] == -1)
				continue;
			FD_SET(fd_list[i],&rdset);
			if(fd_list[i] > maxfd)//顺便寻找最大文件描述符
				maxfd = fd_list[i];
		}

		//每次都需要重新设置一下timeout，因为底层会改变这个值
		struct timeval timeout = {1,0};
		timeout.tv_sec = 1;timeout.tv_usec = 0;

//程序会停在select这里等待,直到被监视的文件句柄有一个或多个发生了状态改变
		//int ret = select(maxfd,&rdset,0,0,&timeout);
		int ret = select(maxfd + 1,&rdset,0,0,NULL);
		switch(ret)
		{
			case -1:
				perror("select");
				break;
			case 0:
			//第一，若将NULL以形参传入，则select函数是阻塞的
			//第二，若时间值设置为0秒0毫秒，则select函数是非阻塞的
			//第三，若时间值大于0,则select函数超时就会返回
				printf("timeout\n");
				break;
			default:
				printf("开始执行serverIO函数\n");
				httpServer::severIO(fd_list,fd_list_size,rdset,connCliNum);
				++connCliNum;
				break;
		}
	}
	return 0;
}
