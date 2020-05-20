#include<unistd.h>

int main()
{	
	char *path = "spider3.py";
	execl(path,path,NULL);
	return 0;
}
