 ///
 /// @file    WordQueryServer.cpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-22 01:55:43
 ///
#include "FileName.hpp"
#include "WordQueryServer.hpp"
#include "WordQuery.hpp"
#include <unistd.h>
#include <string>
using std::string;

namespace
{

string AbsolutelyPath(const string &s)
{
	char *cur=getcwd(NULL,0);
	int loc=s.rfind('/');
	
	string path=s.substr(0,loc);
	chdir(path.c_str());
	string cfgAbsPath=getcwd(NULL,0);
	cfgAbsPath.append("/").append(s.substr(loc+1));
	chdir(cur);
	return cfgAbsPath;
}

void Trim(string &str)
{
	//去除首尾空格
	str[str.size()-1]=' ';//去除最后一个换行符
	str.erase(str.find_last_not_of(' ')+1);
	str.erase(0,str.find_first_not_of(' '));
	for(size_t idx=0;idx!=str.size();++idx)//转换成小写
		if(str[idx]>='A'&&str[idx]<='Z')
			str[idx]+=32;
}

}

namespace wd
{

WordQueryServer::WordQueryServer(const string &cfgFileName)
:_conf(AbsolutelyPath(cfgFileName))
,_wordquery(_conf)
,_threadpool(5,10)
,_tcpserver(_conf.getConfigMap()[IP],
		    atoi(_conf.getConfigMap()[PORT].c_str()))
{ }

void WordQueryServer::start()
{
	_threadpool.start();
	
	_tcpserver.setConnectCallback(std::bind(&WordQueryServer::onConnection,this,std::placeholders::_1));
	_tcpserver.setMessageCallback(std::bind(&WordQueryServer::onMessage,this,std::placeholders::_1));
	_tcpserver.setCloseCallback(std::bind(&WordQueryServer::onClose,this,std::placeholders::_1));

	_tcpserver.start();
}

void WordQueryServer::onConnection(TcpConnectionPtr conn)
{
	printf("%s\n", conn->toString().c_str());
	conn->send("hello, welcome to Chat Server.\r\n");
}

void WordQueryServer::onMessage(TcpConnectionPtr conn)
{
	std::string queryWord(conn->receive());
	Trim(queryWord);//预处理
	
	_threadpool.addTask(std::bind(&wd::WordQueryServer::doQuery,this,conn,queryWord));
}

void WordQueryServer::onClose(TcpConnectionPtr conn)
{
	printf("%s close\n", conn->toString().c_str());
}

void WordQueryServer::doQuery(TcpConnectionPtr & conn,const string & query)
{
	string result=_wordquery.doQuery(query);
	conn->sendInLoop(result);	
}

}

