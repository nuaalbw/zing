/*************************************************************************
	> File Name: TcpConnection.cpp
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Tue 30 Nov 2021 10:11:17 AM CST
 ************************************************************************/

#include "TcpConnection.h"
#include "SocketUtil.h"
#include "../base/Logger.h"

using namespace zing;
using namespace zing::net;
using namespace zing::base;

TcpConnection::TcpConnection(TaskScheduler* ts, int sockfd)
	: ts_(ts), 
	  readBuffer_(new ReadBuffer), 
	  writeBuffer_(new WriteBuffer), 
	  closed_(false), 
	  channel_(new Channel(sockfd))
{
	channel_->setReadCallback(
		std::bind(&TcpConnection::handleRead, this));
	channel_->setWriteCallback(
		std::bind(&TcpConnection::handleWrite, this));
	channel_->setCloseCallback(
		std::bind(&TcpConnection::handleClose, this));
	channel_->setErrorCallback(
		std::bind(&TcpConnection::handleError, this));

	SocketUtil::setNonblock(sockfd);
	SocketUtil::setSendBufSize(sockfd, 100 * 1024);	// 100kb
	SocketUtil::setKeepAlive(sockfd);

	channel_->enableReading();
	ts_->updateChannel(channel_);
}

TcpConnection::~TcpConnection()
{
	int fd = channel_->fd();
	if (fd > 0) {
		SocketUtil::close(fd);
	}
}

void TcpConnection::setReadCallback(ReadCallback cb)
{
	readCallback_ = std::move(cb);
}

void TcpConnection::setCloseCallback(CloseCallback cb)
{
	closeCallback_ = std::move(cb);
}

void TcpConnection::send(std::shared_ptr<char> data, uint32_t size)
{
	if (!closed_) {
		mutex_.lock();
		writeBuffer_->append(data, size);
		mutex_.unlock();
		handleWrite();
	}
}

void TcpConnection::send(const char* data, uint32_t size)
{
	if (!closed_) {
		mutex_.lock();
		writeBuffer_->append(data, size);
		mutex_.unlock();
		handleWrite();
	}
}

void TcpConnection::disconnect()
{
	std::lock_guard<std::mutex> locker(mutex_);

	auto conn = shared_from_this();
	ts_->addTriggerEvent([conn](){ conn->close(); });
}

bool TcpConnection::closed() const
{
	return closed_;
}

int TcpConnection::fd() const
{
	return channel_->fd();
}

uint16_t TcpConnection::port() const
{
	return SocketUtil::getPeerPort(fd());
}

std::string TcpConnection::ip() const
{
	return SocketUtil::getPeerIp(fd());
}

TaskScheduler* TcpConnection::taskScheduler() const
{
	return ts_;
}

void TcpConnection::handleRead()
{
	{
		std::lock_guard<std::mutex> locker(mutex_);

		if (closed_) {
			return;
		}
		int ret = readBuffer_->read(fd());
		if (ret <= 0) {
			LOG_ERROR("handleRead error");
			close();
			return;
		}
	}

	if (readCallback_) {
		bool ret = readCallback_(shared_from_this(), *readBuffer_);
		if (ret == false) {
			std::lock_guard<std::mutex> locker(mutex_);
			close();	
		}
	}
}

void TcpConnection::handleWrite()
{
	if (closed_) {
		return;
	}

	if (!mutex_.try_lock()) {	// 避免阻塞
		return;
	}

	int ret = 0;
	bool empty = false;
	do
	{
		ret = writeBuffer_->send(fd());
		if (ret < 0) {
			close();
			mutex_.unlock();
			return;
		}
		empty = writeBuffer_->empty();
	} while (0);

	if (empty) {	// 若数据全部发送完毕，则不再关注可写事件
		if (channel_->isWriting()) {
			channel_->disableWriting();
			ts_->updateChannel(channel_);
		}
	}
	else if (!channel_->isWriting()) {
		channel_->enableWriting();
		ts_->updateChannel(channel_);
	}

	mutex_.unlock();
}

void TcpConnection::handleClose()
{
	std::lock_guard<std::mutex> locker(mutex_);
	close();
}

void TcpConnection::handleError()
{
	std::lock_guard<std::mutex> locker(mutex_);
	close();
}

void TcpConnection::setDisconnectionCallback(DisconnectionCallback cb)
{
	disconnectionCallback_ = std::move(cb);
}

void TcpConnection::close()
{
	if (closed_) {
		return;
	}

	closed_ = true;
	ts_->removeChannel(channel_);

	if (closeCallback_) {
		closeCallback_(shared_from_this());
	}

	if (disconnectionCallback_) {
		disconnectionCallback_(shared_from_this());
	}
}