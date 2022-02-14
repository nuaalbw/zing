/*************************************************************************
	> File Name: InetAddress.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 14 Feb 2022 10:14:15 PM CST
 ************************************************************************/

#ifndef ZING_NET_INETADDRESS_H
#define ZING_NET_INETADDRESS_H

#include "../base/copyable.h"
#include "../base/StringPiece.h"

#include <netinet/in.h>

namespace zing
{
namespace net
{
namespace sockets
{
const struct sockaddr* sockaddr_cast(const struct sockaddr_in6* addr);
} // namespace sockets

class InetAddress: public zing::copyable
{
public:
	explicit InetAddress(uint16_t port = 0, bool loopbackOnly = false, bool ipv6 = false);

	InetAddress(StringArg ip, uint16_t port, bool ipv6 = false);

	explicit InetAddress(const struct sockaddr_in& addr)
		: addr_(addr)
	{}

	explicit InetAddress(const struct sockaddr_in6& addr)
		: addr6_(addr)
	{}

	sa_family_t family() const { return addr_.sin_family; }
	string toIp() const;
	string toIpPort() const;
	uint16_t port() const;

	const struct sockaddr* getSockAddr() const { return sockets::sockaddr_cast(&addr6_); }
	void setSockAddrInet6(const struct sockaddr_in6& addr6) { addr6_ = addr6; }

	uint32_t ipv4NetEndian() const;
	uint16_t portNetEndian() const { return addr_.sin_port; }

	static bool resolve(StringArg hostname, InetAddress* result);

	void setScopeId(uint32_t scope_id);

private:
	union
	{
		struct sockaddr_in addr_;
		struct sockaddr_in6 addr6_;
	};
	
};
	
} // namespace net
} // namespace zing

#endif