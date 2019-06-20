#include"HttpServer.h"
#include<sys/select.h>

void Usage(const char *str)/*{{{*/
{
	printf("Usage:\t%s ip port\n",str);
}/*}}}*/

void severIO(int *fd_list,const int fd_list_size,fd_set &rdset,int num)/*{{{*/
{
	printf("--------------第%d次进入severIO函数-----------------\n",num);
	for(int i = 0; i < fd_list_size;++i)
	{
		if(fd_list[i] == -1)
			continue;

		//处理服务器端的监听套接字描述符
		if(i == 0 && FD_ISSET(fd_list[i],&rdset)){/*{{{*/
			struct sockaddr_in sock_cli;
			socklen_t len_cli;
			int sockfd_cli = accept(fd_list[i],(sockaddr*)&sock_cli,&len_cli);
			if(sockfd_cli < 0){
				perror("accept");
				continue;
			}
			//得到一个连接，一定不要去读或写，先要把它放到fd_list中
			int k = 0;//rdset和wrset相应位置没有置1,所以可以放在fd_list中
			while(k < fd_list_size && fd_list[k] != -1)++k;
			if(k < fd_list_size)fd_list[k] = sockfd_cli;
			else {printf("fd_list is full\n");close(sockfd_cli);}

			continue;
		}/*}}}*/

		//处理与客户端相关的套接字描述符 的读事件
		if(FD_ISSET(fd_list[i],&rdset)){
			//此时它的读事件已经就绪
			httpServer::getRequestToMap(fd_list[i]);//获取请求并把它存放在一个map表中
//------------------------------------------------------------------------------
			//处理与客户端相关的套接字描述符 的写事件
			//if(FD_ISSET(fd_list[i],&wrset)){/*{{{*/
				//此时它的写事件已经就绪
				Json::Value root;
				Json::Reader read;
				read.parse(httpServer::_request[fd_list[i]],root,false);
	
				if(root["queryString"].asString().size() != 0){//执行cgi
					httpServer::responsePost(fd_list[i]);
				}else{//执行get方法
					httpServer::responeGet(fd_list[i]);
				}
				//读写完之后，关闭该文件描述符，并把它从fd_list中扔掉
				close(fd_list[i]);
				fd_list[i] = -1;
		//	}/*}}}*/
		}
	}
}/*}}}*/

// ./main ip port
int main(int argc,char *argv[])
{
//	if(argc != 3){
//		Usage(argv[0]);
//		exit(1);
//	}

	int sockfd_ser;
	httpServer::startup(sockfd_ser,"192.168.31.121",atoi("8080"));
	//httpServer::startup(sockfd_ser,argv[1],atoi(argv[2]));
	httpServer::statusInit();
	
	int connCliNum = 0;//看看服务器总计服务的连接数目

	int fd_list[1024];
	const int fd_list_size = sizeof(fd_list)/sizeof(fd_list[0]);
	memset(fd_list,0xff,sizeof(fd_list));
	fd_list[0] = sockfd_ser;//第零个位置总是放服务器的监听套接字描述符
	int maxfd = sockfd_ser;//寻找最大的文件描述符

	fd_set rdset/*,wrset*/;//我们关心的事件
	for(;;)
	{
		//每次进来都需要重新设置关心事件，因为rdset,wrset都是输入输出型参数
		FD_ZERO(&rdset);
//      FD_ZERO(&wrset);
		for(int i = 0;i < fd_list_size;++i)
		{
			if(fd_list[i] == -1)
				continue;
			FD_SET(fd_list[i],&rdset);
//			FD_SET(fd_list[i],&wrset);
			if(fd_list[i] > maxfd)//顺便寻找最大文件描述符
				maxfd = fd_list[i];
		}

		//每次都需要重新设置一下timeout，因为底层会改变这个值
		struct timeval timeout = {1,0};
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		//int ret = select(maxfd,&rdset,&wrset,0,&timeout);
		//程序会停在select这里等待,直到被监视的文件句柄有一个或多个发生了状态改变
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
				severIO(fd_list,fd_list_size,rdset,connCliNum);
				++connCliNum;
				break;
		}
	}
	return 0;
}
