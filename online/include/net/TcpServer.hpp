///
/// @file    TcpSever.hpp
/// @author  yangyu/Icot(jobyang@163.com)
/// @date    2017-03-18 00:05:58
///
#ifndef __WD_TCPSERVER_H__
#define __WD_TCPSERVER_H__

#include "Noncopyable.hpp"
#include "EpollPoller.hpp"
#include "Socket.hpp"
#include "InetAddress.hpp"
#include <string>
using std::string;

namespace wd
{

class TcpServer : Noncopyable
{
public:
	typedef EpollPoller::EpollCallback TcpServerCallback;

	TcpServer(const string & ip, unsigned short port);

	void start()
	{ _poller.loop(); }

	void setConnectCallback(TcpServerCallback cb)
	{ _poller.setConnectCallback(std::move(cb)); }
	void setMessageCallback(TcpServerCallback cb)
	{ _poller.setMessageCallback(std::move(cb)); }
	void setCloseCallback(TcpServerCallback cb)
	{ _poller.setCloseCallback(std::move(cb)); }

private:
	InetAddress _addr;
	Socket _sockfd;
	EpollPoller _poller;
};

}
#endif
