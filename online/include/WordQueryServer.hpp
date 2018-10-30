 ///
 /// @file    WordQueryServer.hpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-22 01:37:38
 ///
#ifndef __WD_WORDQUERYSERVER_H__
#define __WD_WORDQUERYSERVER_H__

#include "TcpServer.hpp"
#include "Threadpool.hpp"
#include "TcpConnection.hpp"
#include "Configuration.hpp"
#include "WordQuery.hpp"
#include <memory>

namespace wd
{

class WordQueryServer
:public std::enable_shared_from_this<WordQueryServer>
{
public:
	WordQueryServer(const string &cfgFileName);
	void start();
	void onConnection(TcpConnectionPtr conn);
	void onMessage(TcpConnectionPtr conn);
	void onClose(TcpConnectionPtr conn);
	void doQuery(TcpConnectionPtr & conn,const string & query);
private:
	Configuration    _conf;
	WordQuery		_wordquery;
	Threadpool		_threadpool;
	TcpServer		_tcpserver;
};

}
#endif
