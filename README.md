# 基于C++11的Linux轻量级网络库

### 项目简介

zing是**基于C++11**实现的一款Linux轻量级网路库，是本人在学习完陈硕老师的《Linux多线程服务端编程》中的muduo网络库后，参考其设计思想实现的。在实现过程中，使用C++11的内容去除了对boost库的依赖，简化了base库中的代码实现。网络库沿用了muduo的整体思想，即**one loop per thread + I/O multiplexing**，对原生Sockets API提供了更高层的封装，能够大幅较低Linux环境下网络应用程序的开发难度。

### 测试运行环境

- 系统版本：CentOS Linux release 7.9.2009 
- 编译器版本：g++ 9.3.1

### 技术点

- 基于事件驱动的**Reactor**模式，使用**non-blocking I/O + I/O multiplexing**，程序的基本结构是一个Eventloop，以事件驱动和事件回调的方式实现业务逻辑
- 使用基于**select**和**epoll**的IO多路复用机制，后期会加入**poll**
- 使用C++11中的**智能指针（shared_ptr, unique_ptr, weak_ptr）**管理多线程环境下的对象资源
- 基于**RAII**机制，使用C++11中的**std::lock_guard**和**std::unique_lock**，通过建立**栈上对象**的方式，管理std::mutex资源
- 使用C++11中的**std::bind**，**std::function**和**lambda表达式**，实现了设置函数回调、任务添加等功能
- 基于**单例模式**实现的**日志库**，使用printf(fmt, ...)的风格输出日志，支持设置多种level和目的地，可以设置**运行时过滤器（filter）**，控制不同组件的消息级别和目的地
- 使用**二叉搜索树**（std::set）实现了**TimerQueue**，利用了现有的STL容器库，实现简单，容易验证其正确性，操作复杂度为O(logN)
- 使用**双向链表**结构实现**Memory Pool**，能够根据用户需求从内存池上分配内存或将已分配的内存释放回池中，以减少内存碎片的产生

### 安装与使用

```shell
cd src/
make
```

### 代码示例

这里以echo server的实现为例，echo.h：

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

### 目录结构

- examples：编程示例
- srcs：源代码
