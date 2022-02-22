# 基于C++11的Linux轻量级网络库

### 项目简介

zing是**基于C++11**实现的一款Linux轻量级网路库，是本人学习完muduo网络库后，参考其设计思想实现的。在实现过程中大量使用了C++11中的特性，去除了Boost库的依赖，以简洁的代码对原生Sockets API提供了更高层的封装，能够大幅较低Linux环境下网络应用程序的开发难度。当前写这个库的目的仅是用于学习，后期会不断完善其功能。

### 测试运行环境

- 系统版本：CentOS Linux release 7.9.2009 
- 编译器版本：g++ 9.3.1

### 技术点

- 基于Poll和Epoll实现**one loop per thread + ThreadPool**的多线程并发模型
- 基于**non-blocking I/O + I/O multiplexing**实现基于事件驱动的Reactor模式
- 网络库的基本结构是一个Eventloop，支持用户以事件驱动和事件回调的方式实现业务逻辑
- 使用**智能指针**（shared_ptr, unique_ptr, weak_ptr）和**RAII**机制管理多线程环境下的对象资源
- 使用C++11中的**std::bind**和**std::function**实现设置函数回调、任务添加等功能
- 基于**单例模式**和**双缓冲技术**实现异步**日志库**，用于记录服务器的运行状态，支持设置多种日志级别和目的地
- 使用**红黑树**（std::set）实现了**TimerQueue**，利用了现有的STL容器库，实现简单，容易验证其正确性，操作复杂度为O(logN)
- 使用**双向链表**结构实现**Memory Pool**，能够根据用户需求从内存池上分配内存或将已分配的内存释放回池中，以减少内存碎片的产生

### 代码示例

这里以echo server的实现为例，后期会加入一个HttpServer

echo.h：

```cpp
#ifndef ZING_EXAMPLES_ECHO_H
#define ZING_EXAMPLES_ECHO_H

#include "EventLoop.h"
#include "TcpServer.h"

class EchoServer
{
public:
    EchoServer(zing::net::EventLoop* loop);
    void start(std::string ip, uint16_t port);

private:
    void onConnection(zing::net::TcpConnectionPtr conn);
    bool onMessage(zing::net::TcpConnectionPtr conn,
                   zing::net::ReadBuffer& buffer);

private:
    zing::net::TcpServer server_;
};

#endif
```

echo.cpp：

```cpp
#include "echo.h"
#include "Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

using std::placeholders::_1;
using std::placeholders::_2;

EchoServer::EchoServer(zing::net::EventLoop* loop)
    : server_(loop)
{
    server_.setConnectionCallback(
	std::bind(&EchoServer::onConnection, this, _1));
    server_.setMessageCallback(
	std::bind(&EchoServer::onMessage, this, _1, _2));
}

void EchoServer::start(std::string ip, uint16_t port)
{
    server_.start(ip, port);
}

void EchoServer::onConnection(zing::net::TcpConnectionPtr conn)
{
    LOG_INFO("EchoServer - ip<%s> port<%d>", conn->ip().c_str(), conn->port());
}

bool EchoServer::onMessage(zing::net::TcpConnectionPtr conn,
                           zing::net::ReadBuffer& buffer)
{
    std::string res(buffer.peek(), buffer.readableBytes());
    buffer.retrieveAll();
    conn->send(res.c_str(), res.size());
    return true;
}
```

main.cpp：

```cpp
#include "echo.h"

using namespace zing;
using namespace zing::net;

int main()
{
    EventLoop loop;
    EchoServer server(&loop);
    server.start("127.0.0.1", 12345);
    loop.loop();
}
```