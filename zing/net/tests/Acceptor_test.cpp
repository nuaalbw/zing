/*************************************************************************
	> File Name: Acceptor_test.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 15 Feb 2022 07:50:11 PM CST
 ************************************************************************/

#include "../Acceptor.h"
#include "../EventLoop.h"
#include "../InetAddress.h"
#include "../Socket.h"
#include "../SocketsOps.h"
#include "../../base/CurrentThread.h"

#include <stdio.h>

using namespace zing;
using namespace zing::net;

void newConnection(int sockfd, const InetAddress& peerAddr)
{
	printf("newConnection(): accepted a new connection from %s\n", 
				 peerAddr.toIpPort().c_str());
	::write(sockfd, "How are you?\n", 13);
	sockets::close(sockfd);	
}

int main()
{
	printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	InetAddress listenAddr(9981);
	EventLoop loop;

	Acceptor acceptor(&loop, listenAddr, true);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();
	loop.loop();
}