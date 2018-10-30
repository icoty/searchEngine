///
/// @file    Client.cpp
/// @author  yangyu/Icot(jobyang@163.com)
/// @date    2017-03-17 22:36:29
///
#include "Log4func.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define ERR_EXIT(m) \
	do { \
		perror(m);\
		exit(EXIT_FAILURE);\
	}while(0)

void do_service(int sockfd);

int main(int argc, const char *argv[])
{
#if 1
	char tmp[10]={0};
	char ip[16]={0};
	int port;
	FILE *fp=fopen("/home/yangyu/CPP/spellcorrect/Conf/my.conf","rb");
	if(NULL==fp){
		LogError("fopen error!");
	}
	fscanf(fp,"%s%s",tmp,ip);
	fscanf(fp,"%s%d",tmp,&port);
	fclose(fp);
	int peerfd = socket(PF_INET, SOCK_STREAM, 0);
	if(peerfd == -1)
		ERR_EXIT("socket");

	struct sockaddr_in addr;
	memset(&addr, 0, sizeof addr);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip); //localhost
	addr.sin_port = htons(port);
	socklen_t len = sizeof addr;
	if(connect(peerfd, (struct sockaddr*)&addr, len) == -1)
		ERR_EXIT("Connect");
#endif
#if 0
	int peerfd = socket(PF_INET, SOCK_STREAM, 0);
	if(peerfd == -1)
		ERR_EXIT("socket");
	Socket client(peerfd);
#endif
	do_service(peerfd);
	return 0;
}

void do_service(int sockfd)
{
	char recvbuf[1024] = {0};
	char sendbuf[1024] = {0};
	size_t idx=0;

	int flg=1;
	while(1)
	{
		int flag=1;
		memset(recvbuf, 0, sizeof recvbuf);
		
		if(flg==1)
		{
			int nread = read(sockfd, recvbuf, sizeof recvbuf);
			if(nread == -1)
			{
				if(errno == EINTR)
					continue;
				ERR_EXIT("read");
			}else if(nread == 0){
				LogError("server close!");
				close(sockfd);
				exit(EXIT_SUCCESS);
			}
			printf("receive msg : %s", recvbuf);
		}
		memset(sendbuf, 0, sizeof sendbuf);
		fgets(sendbuf, sizeof sendbuf, stdin);
			
		for(idx=0;idx<strlen(sendbuf)-1;++idx)
			if(sendbuf[idx]!=32){
				flag=0;
				break;
			}

		if(flag==1){
			LogError("invalid input,cannot be all space!");
			flg=0;
			memset(sendbuf, 0, sizeof sendbuf);
		}else if(flag==0){
			flg=1;
			write(sockfd, sendbuf, strlen(sendbuf));
		}
	}
}
