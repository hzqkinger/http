// #include<unistd.h>
// #include<sys/socket.h>
// #include<sys/types.h>
// #include<sys/stat.h>
// #include<sys/wait.h>
// #include<sys/sendfile.h>
// #include<arpa/inet.h>
// #include<fcntl.h>
// #include<errno.h>
// 
// #include<stdio.h>
// #include<iostream>
// using namespace std;
// #include<stdlib.h>
// #include<string.h>
// #include<pthread.h>
// #include<stdlib.h>
// 
// #include<string>
// #include<map>
// 
 #include"HttpServer.h"
#include"Log.h"
#define Max 1024

//socket bind listen
void httpServer::startup(int &sockfd_ser,const char *ip,short port)/*{{{*/
{
	sockfd_ser = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd_ser < 0)
	{
		perror("socket");
		exit(1);
	}
	int opt = 1;
	setsockopt(sockfd_ser, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in sock_ser;
	sock_ser.sin_family = AF_INET;
	sock_ser.sin_port = htons(port);
	sock_ser.sin_addr.s_addr = inet_addr(ip);
	if(bind(sockfd_ser,(struct sockaddr*)&sock_ser,sizeof(sock_ser)) < 0)
	{
		perror("bind");
		exit(2);
	}
	if(listen(sockfd_ser,5) < 0)
	{
		perror("listen");
		exit(3);
	}
}/*}}}*/

//statusInit函数有一个问题，重定向的标准输入，不知道怎么重定向回来
void httpServer::statusInit()//将状态码及其描述放到map表中/*{{{*/
{
	char buf[Max] = {0};
	int fd = open("./Status",O_RDONLY);
	dup2(fd,0);
	while(cin.getline(buf,Max,'#')){
		cin.ignore(Max,'\n');

		int num = atoi(strtok(buf,":"));
		string s = strtok(NULL,":");
		_status[num] = s;
	}
}/*}}}*/

void httpServer::echo_header(int sock,int status)/*{{{*/
{
	char line[1024];
	sprintf(line, "HTTP/1.0 %d %s\r\n",status,_status[status].c_str());
	send(sock, line, strlen(line), 0);
	sprintf(line,"Content-Type:text/html;charset=utf-8\r\n");
	send(sock, line, strlen(line), 0);
	sprintf(line, "\r\n");
	send(sock, line, strlen(line), 0);
}/*}}}*/

void httpServer::clear_header(int sock)/*{{{*/
{
	char line[Max];
	do{
		get_line(sock, line, sizeof(line));
		//printf("%s", line);
	}while(strcmp(line, "\n"));
}/*}}}*/

int httpServer::echo_www(int sock, const char *path, int size)/*{{{*/
{
	int fd = open(path, O_RDONLY);
	if(fd < 0){
		return 404;
	}
	sendfile(sock, fd, NULL, size);
	close(fd);
	return 200;
}/*}}}*/

int httpServer::get_line(int sock, char line[], int num)// line -> \r,\r\n,\n->\n/*{{{*/
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

void httpServer::print_request(int sock)/*{{{*/
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

void httpServer::exec_cgi(int sockfd_cli,char *method,char *path,char *queryString)/*{{{*/
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
		echo_header(sockfd_cli,200);	
		char ch;
		while(read(output[0],&ch,1) > 0){
			send(sockfd_cli,&ch,1,0);
		}
		
		wait(NULL);
	}
}/*}}}*/

//-----------------------------------------------------
void httpServer::getRequestToMap(int sockfd)//获取请求并把它存放在一个map表中/*{{{*/
{
	Json::Value root;
	string str;

	root["homePage"] = "index.html";

	char line[Max];
	get_line(sockfd,line,Max);
	printf("请求行: %s\n",line);//////////////////////////
	//将请求行放到Value对象中去
	//GET /xxxx?yyR=z HTTP1.1     //GET /echoHello.html HTTP1.1
	char *cur = line,*next = line;/*{{{*/
	while(*next != ' ')++next;*next = 0;
	root["method"] = cur;
	cur = ++next;
	bool flag = false;
	while(*next != ' ' && *next != '?')++next;
	if(*next == '?')flag = true;//说明这里有queryString
	*next = 0;
	root["path"] = cur;
	cur = ++next;
	if(flag){
		while(*next != ' ')++next;*next = 0;
		root["queryString"] = cur;
		cur = ++next;
	}
	root["version"] = cur;/*}}}*/

	//如果有，我只关心Content_Length字段；请求头的其它字段不关心
	if(!(root["queryString"].asString().size())){/*{{{*/
		const char *content = "Content-Length: ";
		int content_len = 0;
		//读取请求头，试图获取Content-Length字段信息
		while(get_line(sockfd,line,Max) > 1){
			//如果没有content-length字段，会把空行获取到,所以下面的clear_header会死
			if(strncasecmp(line,content,strlen(content)) == 0){
				content_len = atoi(line+ strlen(content));
				break;
			}
		}
		root["content-length"] = content_len;	
		if(content_len)//如果content_len不为零，则空行没有被获取
			clear_header(sockfd);//清除请求头信息
////////////////////////////////////////////////////////////////////
		//暂时认为post请求只发送一个请求
		//我这里认为content_len小于Max(这是个潜在的bug)
		memset(line,0,sizeof(line));
		for(int i = 0; i < content_len;++i){
			read(sockfd,line + i,1);
		}
		root["queryString"] = line;
	}/*}}}*/

	Json::FastWriter fw;
	httpServer::_request[sockfd] = fw.write(root);
}/*}}}*/
void httpServer::responeGet(int sockfd)/*{{{*/
{
	Json::Value root;//method path queryString version content-length homePage
	Json::Reader read;
	read.parse(httpServer::_request[sockfd],root,false);

	char path[Max/10];
	sprintf(path,"wwwroot/html%s",root["path"].asString().c_str());
	struct stat st;		
	if(stat(path,&st) < 0){
		printf("文件 %s 不存在\n",path);/*{{{*/
//ho_errorPage(sockfd,,);//////////////////////////////
	}else{
		if(S_ISDIR(st.st_mode)){
			strcat(path,root["homePage"].asString().c_str());
			//如果访问的是目录，拼接成该目录下的首页
			stat(path,&st);
		}/*}}}*/
	}
	echo_header(sockfd,200);//发送响应头部
	echo_www(sockfd,path,st.st_size);
//	printf("path:\t%s\n",path);///////////////////////////
}/*}}}*/
void httpServer::responsePost(int sockfd)/*{{{*/
{
	Json::Value root;//method path queryString version content-length homePage
	Json::Reader read;
	read.parse(httpServer::_request[sockfd],root,false);

	//重新拼装path路径，
	char path[Max/10];
	sprintf(path,"wwwroot/cgi%s",root["path"].asString().c_str());
	struct stat st;
	if(stat(path,&st) < 0){
		printf("文件 %s 不存在\n",path);
//ho_errorPage(sockfd,,);//////////////////////////////
	}else{
		if(!(st.st_mode & S_IXOTH)){//判断该文件的可执行
//ho_errorPage(sockfd,,);//////////////////////////////
		}
	}
	char *method = (char*)root["method"].asString().c_str();
	char *queryString = (char*)root["queryString"].asString().c_str();
	exec_cgi(sockfd,method,path,queryString);
}/*}}}*/

//-----------------------------------------------------
void httpServer::requestInit(int sockfd_cli,char *line,char *&method,char *&url,char *&queryString,char *&version)/*{{{*/
{
	int lineIndex = 0;

	//GET /xxxx?yyR=z HTTP1.1     //GET /echoHello.html HTTP1.1
	get_line(sockfd_cli,line,Max);
	printf("line:\t%s\n",line);//////////////////////////////////

	method = line;//method --> @GET

	while(lineIndex < Max && line[lineIndex] != ' ')
		++lineIndex;
	line[lineIndex++] = '\0';
	url = line + lineIndex;//url --> @/xxxx?yy=z

	while(lineIndex < Max && line[lineIndex] != '?' && line[lineIndex] != ' ')
		++lineIndex;//queryString --> @yy=z
	queryString = line[lineIndex] == '?' ? line + lineIndex + 1 : NULL;
	line[lineIndex++] = '\0';

	while(queryString && lineIndex < Max && line[lineIndex] != ' ')
		++lineIndex;//如果queryString不为空，则会循环几次
	version = line + lineIndex;//version --> @HTTP1.1/n  
}/*}}}*/
void httpServer::responeGet(int sockfd_cli,char *path,char *url,const char *homePage)/*{{{*/
{
	clear_header(sockfd_cli);//清除请求头部,此时我不再处理请求头部了
	
//                             最先判断文件或目录是否存在
//下面的判断语句，可以先sprintf，然后判断是文件还是目录，是目录就在strcat一个homePage
	sprintf(path,"wwwroot/html%s",url);//考虑路径是文件还是目录
	struct stat st;		
	if(stat(path,&st) < 0){
		printf("文件 %s 不存在\n",path);/*{{{*/
//ho_errorPage(sockfd_cli,,);//////////////////////////////
		}else{
			if(S_ISDIR(st.st_mode)){
				strcat(path,homePage);//如果访问的是目录，拼接成该目录下的首页
				stat(path,&st);
			}/*}}}*/
		}
	echo_header(sockfd_cli,200);//发送响应头部
	echo_www(sockfd_cli,path,st.st_size);
//	printf("path:\t%s\n",path);///////////////////////////
}/*}}}*/
void httpServer::responsePost(int sockfd_cli,char *method,char *path,char *url,char *&queryString)/*{{{*/
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
	sprintf(path,"wwwroot/cgi%s",url);
	struct stat st;
	if(stat(path,&st) < 0){
		printf("文件 %s 不存在\n",path);
//ho_errorPage(sockfd_cli,,);//////////////////////////////
	}else{
		if(!(st.st_mode & S_IXOTH)){//判断该文件的可执行
//ho_errorPage(sockfd_cli,,);//////////////////////////////
		}
	}
	exec_cgi(sockfd_cli,method,path,queryString);
}/*}}}*/

void *httpServer::worker(void *arg)/*{{{*/
{
	int sockfd_cli = *(int*)arg;
	const char *homePage = "index.html";

	char line[Max] = {0};//刚开始，得到请求行
	char path[Max/10] = {0};//这里存储初始化的路径
	char *url,*queryString,*version,*method = line;
	//得到请求行,并解析出相应的字段
	//GET /xxxx?yyR=z HTTP1.1     //GET /echoHello.html HTTP1.1
	requestInit(sockfd_cli,line,method,url,queryString,version);
	printf("----------------------------------------------------------\n");

//  GET方法，quertString==NULL时，直接返回页面
//           不为空时,执行cgi程序
//  POST方法，直接执行cgi程序
	if(strcmp(method,"POST") == 0 || queryString)//执行cgi程序
	{
		responsePost(sockfd_cli,method,path,url,queryString);
	}else if(strcmp(method,"GET") == 0){//直接返回用户请求的页面即可
		responeGet(sockfd_cli,path,url,homePage);
	}else{
		//to do others
	}

	close(sockfd_cli);
}/*}}}*/
	
map<int,string> httpServer::_status;//静态变量初始化
map<int,string> httpServer::_request;//静态变量初始化
