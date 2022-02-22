/*************************************************************************
	> File Name: TcpServer.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 18 Feb 2022 08:10:28 PM CST
 ************************************************************************/

#ifndef ZING_NET_TCPSERVER_H
#define ZING_NET_TCPSERVER_H

#include "../base/Atomic.h"
#include "../base/Types.h"
#include "TcpConnection.h"

#include <map>

namespace zing
{
namespace net
{

class Acceptor;
class EventLoop;
class EventLoopThreadPool;

class TcpServer : noncopyable
{
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;
  enum Option
  {
    kNoReusePort,
    kReusePort,
  };

  TcpServer(EventLoop* loop,
            const InetAddress& listenAddr,
            const string& nameArg,
            Option option = kNoReusePort);
  ~TcpServer();

  const string& ipPort() const { return ipPort_; }
  const string& name() const { return name_; }
  EventLoop* getLoop() const { return loop_; }

  void setThreadNum(int numThreads);	// 设置IO线程的个数
  void setThreadInitCallback(const ThreadInitCallback& cb)
  { threadInitCallback_ = cb; }
  std::shared_ptr<EventLoopThreadPool> threadPool()
  { return threadPool_; }

  void start();

  void setConnectionCallback(const ConnectionCallback& cb)
  { connectionCallback_ = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { messageCallback_ = cb; }

  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { writeCompleteCallback_ = cb; }

private:
  void newConnection(int sockfd, const InetAddress& peerAddr);
  void removeConnection(const TcpConnectionPtr& conn);
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

	using ConnectionMap = std::map<string, TcpConnectionPtr>;

  EventLoop* loop_;  // the acceptor loop
  const string ipPort_;
  const string name_;
  std::unique_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
  std::shared_ptr<EventLoopThreadPool> threadPool_;
  ConnectionCallback connectionCallback_;
  MessageCallback messageCallback_;
  WriteCompleteCallback writeCompleteCallback_;
  ThreadInitCallback threadInitCallback_;
  AtomicInt32 started_;
  // always in loop thread
  int nextConnId_;
  ConnectionMap connections_;
};

}  // namespace net
}  // namespace zing

#endif