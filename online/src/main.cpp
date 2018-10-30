///
/// @file    main.cpp
/// @author  yangyu/Icot(jobyang@163.com)
/// @date    2017-03-18 00:09:27
///
#include "Log4func.hpp"
#include "WordQueryServer.hpp"
#include <iostream>
using std::cout;
using std::endl;

int main(int argc,char **argv)
{
	if(argc!=2){
		LogError("input my.conf path!");
		return -1;
	}
	cout<<argv[1]<<endl;
	
	wd::WordQueryServer query(argv[1]);
	query.start();
	return 0;
}

#if 0
void onConnection(const wd::TcpConnectionPtr &conn)
{
	printf("%s\n", conn->toString().c_str());
	conn->send("hello, welcome to Chat Server.\r\n");
}

void onMessage(const wd::TcpConnectionPtr &conn)
{
	std::string s(conn->receive());
	//Function task(s,conn);
	//threadpoll.addTask(task);//IO线程 --> 计算线程

	conn->sendInLoop(s);
}

void onClose(const wd::TcpConnectionPtr &conn)
{
	printf("%s close\n", conn->toString().c_str());
}

int main(int argc, char const *argv[])
{
	//class Server.cc内部create sockfd
	char ip[16]={0};
	int port;
	FILE *fp=fopen("/home/icot/c++/reactive_reactor_170317/tcp_epoll/Conf/IpPort.conf","rb");
	if(fp == NULL)
		printf("fopen error!\n");
	fscanf(fp,"%s%d",ip,&port);
	fclose(fp);
	wd::TcpServer server("192.168.118.130", 9999);//初始化InetAddress
	server.setConnectCallback(&onConnection);
	server.setMessageCallback(&onMessage);
	server.setCloseCallback(&onClose);
	server.start();
	
	if(argc!=2){
		LogError("input my.conf path!");
		return -1;
	}
	
	wd::SpellCorrectServer spell(argv[1]);
	spell.start();

	return 0;
}
#endif
