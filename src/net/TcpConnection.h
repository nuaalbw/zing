/*************************************************************************
	> File Name: TcpConnection.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 09:34:20 AM CST
 ************************************************************************/

#ifndef ZING_NET_TCP_CONNECTION_H
#define ZING_NET_TCP_CONNECTION_H

#include "../base/noncopyable.h"
#include "TaskScheduler.h"
#include "ReadBuffer.h"
#include "WriteBuffer.h"
#include "Channel.h"
#include <memory>

namespace zing
{
namespace net
{
	
class TcpConnection
	: noncopyable, 
	  public std::enable_shared_from_this<TcpConnection>
{
	using ReadCallback = std::function<bool(
		std::shared_ptr<TcpConnection>, ReadBuffer&)>;

	using CloseCallback = std::function<void(
		std::shared_ptr<TcpConnection>)>;
	
	using DisconnectionCallback = std::function<void(
		std::shared_ptr<TcpConnection>)>;

	using ReadBufferPtr = std::unique_ptr<ReadBuffer>;
	using WriteBufferPtr = std::unique_ptr<WriteBuffer>;

public:
	TcpConnection(TaskScheduler* ts, int sockfd);
	virtual ~TcpConnection();

	void setReadCallback(ReadCallback cb);
	void setCloseCallback(CloseCallback cb);
	void send(std::shared_ptr<char> data, uint32_t size);
	void send(const char* data, uint32_t size);
	void send(std::string data);
	void send(const zing::net::ReadBuffer& buffer);
	void disconnect();

	bool closed() const;
	int fd() const;
	uint16_t port() const;
	std::string ip() const;
	TaskScheduler* taskScheduler() const;

protected:
	friend class TcpServer;

	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleClose();
	virtual void handleError();
	void setDisconnectionCallback(DisconnectionCallback cb);

	TaskScheduler* ts_;
	ReadBufferPtr readBuffer_;
	WriteBufferPtr writeBuffer_;
	std::atomic_bool closed_;

private:
	void close();

	ChannelPtr channel_;
	std::mutex mutex_;
	DisconnectionCallback disconnectionCallback_;
	CloseCallback closeCallback_;
	ReadCallback readCallback_;
};

} // namespace net
} // namespace zing

#endif