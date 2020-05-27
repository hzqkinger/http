#include"HttpServer.h"
#include<epoll.h>

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
	//httpServer::startup(sockfd_ser,"192.168.43.236",atoi("8080"));
	httpServer::startup(sockfd_ser,argv[1],atoi(argv[2]));
	httpServer::statusInit();
	
	int connCliNum = 0;//看看服务器总计服务的连接数目

	int epfd = epoll_create(256);
	struct epoll_event ev,ev_arr[20];//ev_arr数组是用来装就绪事件的
	ev.events = EPOLLIN;
	ev.data.fd = sockfd_ser;//服务器端的监听套接字描述符
	epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd_ser,&ev);//将监听套接字描述符插入到epoll树中

	for(;;)
	{
		int timeout = 1000;//设置一下超时时间

//程序会停在epoll这里等待,直到被监视的文件句柄有一个或多个发生了状态改变
		int ret = epoll_wait(epfd,ev_arr,sizeof(ev_arr)/sizeof(ev_arr[0]);timeout);
		switch(ret)
		{
			case -1:
				perror("epoll");
				break;
			case 0:
			//第一，若将NULL以形参传入，则select函数是阻塞的
			//第二，若时间值设置为0秒0毫秒，则select函数是非阻塞的
			//第三，若时间值大于0,则select函数超时就会返回
				printf("timeout\n");
				break;
			default:
				printf("开始执行serverIO函数\n");
				httpServer::severIO(epfd,ev_arr,ret,sockfd_ser);
				++connCliNum;
				break;
		}
	}
	return 0;
}
