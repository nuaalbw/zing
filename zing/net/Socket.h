/*************************************************************************
	> File Name: Socket.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Mon 14 Feb 2022 11:26:12 PM CST
 ************************************************************************/

#ifndef ZING_NET_SOCKET_H
#define ZING_NET_SOCKET_H

#include "../base/noncopyable.h"

struct tcp_info;	// in <netinet/tcp.h>

namespace zing
{
namespace net
{

class InetAddress;

// 文件描述符的封装
class Socket: noncopyable
{
public:
	explicit Socket(int sockfd)
		: sockfd_(sockfd)
	{}

	~Socket();

	int fd() const { return sockfd_; }

	// return true if success
	bool getTcpInfo(struct tcp_info*) const;
	bool getTcpInfoString(char* buf, int len) const;

	void bindAddress(const InetAddress& localaddr);

	void listen();

	/// On success, returns a non-negative integer that is
  /// a descriptor for the accepted socket, which has been
  /// set to non-blocking and close-on-exec. *peeraddr is assigned.
  /// On error, -1 is returned, and *peeraddr is untouched.
  int accept(InetAddress* peeraddr);

  void shutdownWrite();

  ///
  /// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
  ///
  void setTcpNoDelay(bool on);

  ///
  /// Enable/disable SO_REUSEADDR
  ///
  void setReuseAddr(bool on);

  ///
  /// Enable/disable SO_REUSEPORT
  ///
  void setReusePort(bool on);

  ///
  /// Enable/disable SO_KEEPALIVE
  ///
  void setKeepAlive(bool on);

private:
	const int sockfd_;
};
	
} // namespace net
} // namespace zing

#endif