/*************************************************************************
    > File Name: test.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月02日 星期日 16时07分37秒
 ************************************************************************/
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
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define Max 1024

bool startup(int &sockfd_ser)/*{{{*/
{
	int opt = 1;
	setsockopt(sockfd_ser, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockfd_ser = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd_ser < 0)
	{
		perror("socket");
		return false;
		//exit(1);
	}
	struct sockaddr_in sock_ser;
	sock_ser.sin_family = AF_INET;
	sock_ser.sin_port = htons(8080);
	sock_ser.sin_addr.s_addr = inet_addr("192.168.31.121");
	if(bind(sockfd_ser,(struct sockaddr*)&sock_ser,sizeof(sock_ser)) < 0)
	{
		perror("bind");
		return false;
		//exit(2);
	}
	if(listen(sockfd_ser,5) < 0)
	{
		perror("listen");
		return false;
		//exit(3);
	}
	return true;
}/*}}}*/

//void echo_hello(int &sockfd_cli)/*{{{*/
//{
//	char line[1024];
//	
//    sprintf(line, "HTTP/1.0 200 OK\r\n");
//    send(sockfd_cli, line, strlen(line), 0);
//    sprintf(line,"Content-Type:text/html;charset=ISO-8859-1\r\n");
//    send(sockfd_cli, line, strlen(line), 0);
//    sprintf(line, "\r\n");
//    send(sockfd_cli, line, strlen(line), 0);
//
//    sprintf(line, "<h1> hello<input type=\"text\" name=\"school\" value=\"\">\
//					<br><br><input type=\"submit\" value=\"eee\"></h1>\r\n");
//	send(sockfd_cli,line,strlen(line),0);
//}/*}}}*/

void echo_header(int sock)/*{{{*/
{
	char line[1024];
	sprintf(line, "HTTP/1.0 200 OK\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line,"Content-Type:text/html;charset=utf-8\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "\r\n");
	send(sock, line, strlen(line), 0);
}/*}}}*/

int echo_www(int sock, const char *path, int size)/*{{{*/
{
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		return 404;
	}
	sendfile(sock, fd, NULL, size);
	close(fd);
	return 200;
}/*}}}*/

int get_line(int sock, char line[], int num)// line -> \r,\r\n,\n->\n/*{{{*/
{
	int i = 0;
	char c = 'A';
	while( c != '\n' && i < num -1 ){
		ssize_t s = recv(sock, &c, 1, 0);
		if(s > 0){//	\r, \r\n -> \n/*{{{*/
			if(c == '\r'){
				recv(sock, &c, 1, MSG_PEEK);
				if( c == '\n'){
					recv(sock, &c, 1, 0);
				}
				else{
					c = '\n';
				}
			}
			//else
			//\n, non \n
			line[i++] = c;
		}/*}}}*/
	}
	line[i] = '\0';
	return i;//返回收到字符的个数
}/*}}}*/

void clear_header(int sock)/*{{{*/
{
	char line[Max];
	do{
		get_line(sock, line, sizeof(line));
		//printf("%s", line);
	}while(strcmp(line, "\n"));
}/*}}}*/

void print_request(int sock)/*{{{*/
{
	char line[1024];
	const char *content_length = "Content-Length";
	int body_size = 0;
	while(get_line(sock,line,1024) > 1)
	{
		//#include<stdlib.h>  strncmp
		//#include<string.h>  strncasecmp
		if(!strncasecmp(line,content_length,strlen(content_length)))
		{//判断有没有Content-Length字段;返回0表示有，
			body_size = atoi(line + strlen(content_length) + 1);
		}
		printf("%s",line );
	}
	printf("-------   body_size = %d   ---------------------\n",body_size);
// 读取body数据，如果最后一行不是空格不是空行，说明是有body数据，
// 空行后面的就是body数据。
	char ch;
	while(body_size--)
	{
		recv(sock,&ch,1,0);
		printf("%c",ch);//printf函数是行缓冲
	}
	fflush(stdout);//方法一：刷新缓冲区
	//printf("\n");//方法二：输出一个换行(会多输出一个换行符，不好)
}/*}}}*/

void exec_cgi(int sockfd_cli,char *method,char *path,char *queryString)/*{{{*/
{
	//此时 path是合法的,quertString 都是合法的
	setenv("queryString",queryString,1);//设置环境变量

	int output[2];
	pipe(output);//从儿子流到父亲

	pid_t pid;
	if((pid = fork()) < 0)
	{
		perror("fork");
		return;
	}else if(pid == 0){//child
		close(output[0]);//关闭读端
		dup2(output[1],1);
		execl(path,path,NULL);
		printf("我不应该被打印出来\n");
	}else{//father
		close(output[1]);//关闭写端
		echo_header(sockfd_cli);	
		char ch;
		while(read(output[0],&ch,1) > 0){
			send(sockfd_cli,&ch,1,0);
		}
		
		wait(NULL);
	}
}/*}}}*/

void *worker(void *arg)/*{{{*/
{
	int sockfd_cli = *(int*)arg;
	const char *homePage = "index.html";
	char line[Max] = {0};
	int lineIndex = 0;
	char *url,*queryString,*version,*method = line;

	//得到请求行,并解析出相应的字段
	//GET /xxxx?yyR=z HTTP1.1     //GET /echoHello.html HTTP1.1
	get_line(sockfd_cli,line,Max);/*{{{*/
	printf("line:\t%s\n",line);//////////////////////////////////
	while(lineIndex < Max && line[lineIndex] != ' ')
		++lineIndex;//method --> GET
	line[lineIndex++] = '\0';
	url = line + lineIndex;//url --> /xxxx
	while(lineIndex < Max && line[lineIndex] != '?' && line[lineIndex] != ' ')
		++lineIndex;
	queryString = line[lineIndex] == '?' ? line + lineIndex + 1 : NULL;
	line[lineIndex++] = '\0';
	version = line + lineIndex;//version --> HTTP1.1/n  
//	printf("method:\t%s\n",method);//////////////////////////////////
//	printf("url:\t%s\n",url);//////////////////////////////////
//	printf("version:\t%s\n",version);//////////////////////////////////
//	if(queryString)printf("%s\n",queryString);////////////////////////////*}}}*/

	//得到初始状态下的请求路径
	char path[Max/10] = {0};
	char *pathPtr = url;
	while(*pathPtr != '/')++pathPtr;

//  GET方法，quertString==NULL时，直接返回页面
//           不为空时,执行cgi程序
//  POST方法，直接执行cgi程序
	if(strcmp(method,"POST") == 0 || queryString)//执行cgi程序
	{
		char buff[Max] = {0};//这个buff是用来装放在body中的queryString的

		//情况一：POST方法且queryString在body中，则一定会有Content-Length字段
		//情况二：GET方法且queryString在url中，则此时queryString不为空
		//情况三：POST放法且queryString在url中，则此时queryString不为空
		//情况四：GET方法且queryString在body中，则一定会有Content-Length字段
		if(NULL == queryString){//注意，情况三和四很少见，我这里就不支持了
			const char *content = "Content-Length: ";
			int content_len = 0;
			//读取请求头，试图获取Content-Length字段信息
			while(get_line(sockfd_cli,buff,Max) > 1){
				if(strncasecmp(buff,content,strlen(content)) == 0){
					content_len = atoi(buff + strlen(content));
					break;
				}
			}	
			clear_header(sockfd_cli);//清除请求头信息

			//我这里认为content_len小于Max(这是个潜在的bug)
			for(int i = 0; i < content_len;++i){
				read(sockfd_cli,buff + i,1);
			}
			queryString = buff;//重新赋值
		}
		//重新拼装path路径，
		sprintf(path,"wwwroot/cgi%s",pathPtr);
		struct stat st;
		if(stat(path,&st) < 0){
			printf("文件 %s 不存在\n",path);
			//echo_errorPage(sockfd_cli,,);//////////////////////////////
		}else{
			if(!(st.st_mode & S_IXOTH)){//判断该文件的可执行
				//echo_errorPage(sockfd_cli,,);//////////////////////////////
				return NULL;//最好不要在这里这样退出，可以返回一个错误页
			}
		}

		exec_cgi(sockfd_cli,method,path,queryString);////////
	}else if(strcmp(method,"GET") == 0){//直接返回用户请求的页面即可
		clear_header(sockfd_cli);//清除请求头部,此时我不再处理请求头部了/*{{{*/
	
//                               最先判断文件或目录是否存在
//下面的判断语句，可以先sprintf，然后判断是文件还是目录，是目录就在strcat一个homePage
		sprintf(path,"wwwroot/html%s",pathPtr);//考虑路径是文件还是目录
		struct stat st;		
		if(stat(path,&st) < 0){
			printf("文件 %s 不存在\n",path);
	//		if(errno & ENOENT)/*{{{*/
	//			printf("文件真的不存在\n");
	//		if(errno & EACCES)
	//			printf("存取文件时被拒绝\n");
			//echo_errorPage(sockfd_cli,,);//////////////////////////////
			//exit(2);服务器中不要出现这句话/*}}}*/
		}else{
// S_IROTH 00004             其他用户具可读取权限/*{{{*/
			if(!(st.st_mode & S_IROTH)){//判断该文件的可读性
				//echo_errorPage(sockfd_cli,,);//////////////////////////////
				return NULL;//最好不要在这里这样退出，可以返回一个错误页
			}
			if(S_ISDIR(st.st_mode)){
				strcat(path,homePage);//如果访问的是目录，拼接成该目录下的首页
				stat(path,&st);
			}
		}
		echo_header(sockfd_cli);//发送响应头部//////////////////////////
		echo_www(sockfd_cli,path,st.st_size);
		printf("path:\t%s\n",path);///////////////////////////////////*}}}*//*}}}*/
	}else{
		//to do others
	}

//	print_request(sockfd_cli);//打印客户端的请求信息/*{{{*/
////		echo_hello(sockfd_cli);//发送响应报文
//		echo_header(sockfd_cli);
//		struct stat st;
////----------------------------------------------------------------------
////		const char *css_path = "wwwroot/css/test24.css";
////		stat(css_path,&st);
////		echo_www(sockfd_cli,css_path,st.st_size);
////
//		const char *html_path = "wwwroot/html/echoHello.html";
//		stat(html_path,&st);
//		echo_www(sockfd_cli,html_path,st.st_size);
////----------------------------------------------------------------------/*}}}*/
		close(sockfd_cli);
}/*}}}*/

int main()/*{{{*/
{
	int sockfd_ser;
	while(!startup(sockfd_ser))
		sleep(1);//socket bind listen

	int connCliNum = 0;
	for(;;)
	{
		int sockfd_cli;

		struct sockaddr_in sock_cli;
		socklen_t sock_len_cli;	
		if((sockfd_cli = accept(sockfd_ser,\
			(struct sockaddr*)&sock_cli,&sock_len_cli)) < 0)
		{
			perror("accept");
			continue;
		}
		printf("already get the %d client\n",++connCliNum);

		pthread_t tid;
		pthread_create(&tid,NULL,worker,(void*)&sockfd_cli);
		pthread_detach(tid);
	}
	return 0;
}/*}}}*/
