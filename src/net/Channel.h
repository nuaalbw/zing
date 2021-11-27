/*************************************************************************
	> File Name: Channel.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Sun 21 Nov 2021 04:11:56 PM CST
 ************************************************************************/

#ifndef ZING_NET_CHANNEL_H
#define ZING_NET_CHANNEL_H

#include "SocketFiles.h"
#include "../base/noncopyable.h"
#include <functional>
#include <memory>

namespace zing
{
namespace net
{

enum EventType
{
	EVENT_NONE	= 0, 
	EVENT_IN	= 1, 
	EVENT_PRI	= 2, 
	EVENT_OUT	= 4, 
	EVENT_ERR	= 8, 
	EVENT_HUP	= 16, 
	EVENT_RDHUP	= 8192
};

// 作用：将IO multiplexing拿到的IO事件分发给不同的事件处理函数
class Channel: noncopyable
{
public:
	using EventCallback = std::function<void(void)>;

	explicit Channel(int sockfd);
	virtual ~Channel();

	// 将不同的事件分发给不同的回调
	void setReadCallback(EventCallback cb);
	void setWriteCallback(EventCallback cb);
	void setCloseCallback(EventCallback cb);
	void setErrorCallback(EventCallback cb);

	// 获取socket fd
	int fd() const;
	// 获取监听的事件
	int events() const;
	// 设置监听的事件
	void setEvents(int events);

	// 设置监听
	void enableReading();
	void enableWriting();
	void disableReading();
	void disableWriting();

	bool isNoneEvent() const;
	bool isWriting() const;
	bool isReading() const;

	void handleEvent(int events);

private:
	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
	int sockfd_;
	int events_;
};
	
using ChannelPtr = std::shared_ptr<Channel>;

} // namespace net
} // namespace zing

#endif