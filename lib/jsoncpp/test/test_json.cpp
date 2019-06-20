/*************************************************************************
    > File Name: test_json.cpp
    > Author: 黄志强
    > Mail: 2069085311@qq.com 
    > Created Time: 2019年06月19日 星期三 14时54分13秒
 ************************************************************************/

#include<iostream>
#include<stdio.h>
using namespace std;
#include"json/json.h"
#include<string>

//①  Json::Value对象  与  std::string对象  之间相互转换
//
//②  Json::Writer对象的write方法 将Value对象转换为string对象
//  std::string FastWriter::write(const Value &root);
//  std::string StyledWriter::write(const Value &root);
//
//③  Json::Reader对象的parse方法 将string对象转换为Value对象
// bool Reader::parse(const std::string &document,Value &root,bool collectComments);
// bool Reader::parse(std::istream& sin,Value &root,bool collectComments);
// bool Reader::parse(const char *beginDoc, const char *endDoc,Value &root,bool collectComments);
void test()/*{{{*/
{
//  Value 
	Json::Value root;
	root["name"] = "hzq";
	root["age"] = "18";
	cout << "Json::Value root " << root << endl;

//  FastWriter StyleWriter
//  std::string FastWriter::write(const Value &root);
//  std::string StyledWriter::write(const Value &root);
	Json::FastWriter fw;
	string str;
	str = fw.write(root);
	cout << "Json::FastWriter str " << str; 
	Json::StyledWriter sw;
	str = sw.write(root);
	cout << "Json::StyledWriter str " << str;

	// Json::Value tmp = str;
	// cout << "Json::Value " << tmp << endl;

//  Reader 
// bool Reader::parse(const std::string &document,Value &root,bool collectComments);
// bool Reader::parse(std::istream& sin,Value &root,bool collectComments);
// bool Reader::parse(const char *beginDoc, const char *endDoc,Value &root,bool collectComments);

	Json::Reader r;
	cout << "parse的返回值 " << r.parse(str,root,false) << endl;
	cout << "Json::Reader root " << root << endl;
}/*}}}*/

void test2(Json::Value &root)/*{{{*/
{
	char line[12] = "xxage";
	char buff[] = "helleeaee\n";
	buff[3] = 0;
	
	root[line] = buff;
	root["xx"] = buff + 4;
	cout << root["xx"].asString() << endl;
	cout << "999999999999999" <<endl;

}
void test22()
{
	Json::Value root;
	test2(root);
	cout << root;
}/*}}}*/

void test3()/*{{{*/
{
	Json::Value tmp;
	tmp["name"] = "xx";
	tmp["age"] = 123;
	cout << "name: " << tmp["name"].asString() << endl;
	cout << "age: " << tmp["age"].asInt() << endl;
	cout << "xx: " << tmp["xx"].asString() << endl;
}/*}}}*/

int main()
{
//	test();
//	test22();	
//	test3();
	Json::Value root;

	////cout << root["queryString"].asString().size() << endl;
	//if(!(root["queryString"].asString().size())){
	//	cout << "+++++++++++++++++++++++++++++++++++"<< endl;
	//}
	root["path"] = "/hah.txt\n";
	char path[100];
	sprintf(path,"wwwroot/html%s",root["path"].asString().c_str());
	printf("%s\n",path);

	return 0;
}
