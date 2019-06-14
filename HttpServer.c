#include"HttpServer.h"

void Usage(const char *str)/*{{{*/
{
	printf("Usage:\t%s ip port\n",str);
}/*}}}*/


// ./httpserver ip port
int main(int argc,char *argv[])
{
	if(argc != 3){
		Usage(argv[0]);
		exit(1);
	}

	int sockfd_ser;
	printf("++++++++++++++++++++++++++++\n");
	httpServer::startup(sockfd_ser,argv[1],atoi(argv[2]));
	printf("++++++++++++++++++++++++++++\n");
	httpServer::statusInit();
	printf("++++++++++++++++++++++++++++\n");
	
	int connCliNum = 0;//看看服务器总计服务的连接数目
	for(;;)
	{
		int sockfd_cli;

		struct sockaddr_in sock_cli;
		socklen_t sock_len_cli;	
		if((sockfd_cli = accept(sockfd_ser,\
			(struct sockaddr*)&sock_cli,&sock_len_cli)) < 0)
		{
			printf("accepti error\n");
			continue;
		}
		printf("already get the %d client\n",++connCliNum);

		pthread_t tid;
		pthread_create(&tid,NULL,httpServer::worker,(void*)&sockfd_cli);
		pthread_detach(tid);
	}
	return 0;
}

