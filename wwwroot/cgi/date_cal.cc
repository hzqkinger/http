/*************************************************************************
    > File Name: date_cal.c
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月04日 星期二 17时31分22秒
 ************************************************************************/

#include"date_cal.hpp"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

//queryString="date1=2019-6-11&date2=2019-9-1"
void getStringFromEnv()
{
	char date[40] = {0};
//	"date1=2019-6-11&date2=2019-9-1"
	setenv("queryString","date1=2019-6-11&date2=2019-9-1",0);////仅仅用在测试
	sprintf(date,getenv("queryString"));
//	printf("queryString:\t%s\n",date);//////////////

	const int date_size = strlen(date);
	int dateIndex = 0;

	while(date[dateIndex++] != '=');/*{{{*/
	char *day,*month,*year = date + dateIndex;
	while(dateIndex < date_size && date[dateIndex] != '-')
		++dateIndex;
	date[dateIndex++] = '\0';
	month = date + dateIndex;
	while(dateIndex < date_size && date[dateIndex] != '-')
		++dateIndex;
	date[dateIndex++] = '\0';
	day = date + dateIndex;
	Date d1(atoi(year),atoi(month),atoi(day));/*}}}*/
	printf("<h2>date1:\t");
	d1.Display();
	printf("</h2>\n");

	while(date[dateIndex++] != '=');year = date + dateIndex;/*{{{*/
	while(dateIndex < date_size && date[dateIndex] != '-')
		++dateIndex;
	date[dateIndex++] = '\0';
	month = date + dateIndex;
	while(dateIndex < date_size && date[dateIndex] != '-')
		++dateIndex;
	date[dateIndex++] = '\0';
	day = date + dateIndex;
	Date d2(atoi(year),atoi(month),atoi(day));/*}}}*/
	printf("<h2>date2:\t");
	d2.Display();
	printf("</h2>\n");

	printf("<h2>相差天数:\t%d</h2>\n",d2-d1);
}
int main()
{
	getStringFromEnv();
	return 0;
}
