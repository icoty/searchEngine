 ///
 /// @file    InetAddress.hpp
 /// @author  yangyu/Icot(jobyang@163.com)
 /// @date    2017-03-17 19:37:08
 ///
#ifndef __WD_INETADDRESS_H__
#define __WD_INETADDRESS_H__

#include <string>
#include <netinet/in.h>
using std::string;

namespace wd
{

class InetAddress{
public:
	explicit InetAddress(unsigned short port);
	InetAddress(const string &ip, unsigned short port);
	InetAddress(const struct sockaddr_in &addr);

	void setSockAddrInet(const struct sockaddr_in &addr);
	const struct sockaddr_in *getSockAddrInet()const;
	string toIp()const;
	unsigned short toPort()const;
private:
	struct sockaddr_in _addr;
};

}
#endif
