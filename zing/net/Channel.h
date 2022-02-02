/*************************************************************************
	> File Name: Channel.h
	> Author: lbw
	> Mail: nuaalbw@163.com 
	> Created Time: Fri 28 Jan 2022 07:52:32 PM CST
 ************************************************************************/

#ifndef ZING_NET_CHANNEL_H
#define ZING_NET_CHANNEL_H

#include "../base/noncopyable.h"
#include "../base/Timestamp.h"

#include <functional>
#include <memory>

namespace zing
{
namespace net
{

class EventLoop;

// 负责将IO复用拿到的事件分发至不同的文件描述符的事件处理函数中
// Channel并不拥有文件描述符，即不负责管理文件描述符的生命周期
// 文件描述符(fd)可以是 socketfd, eventfd, timerfd或是signalfd
class Channel: noncopyable
{
public:
	using EventCallback = std::function<void()>;
	using ReadEventCallback = std::function<void(Timestamp)>;

	Channel(EventLoop* loop, int fd);
	~Channel();

	void handleEvent(Timestamp receiveTime);

	void setReadCallback(ReadEventCallback cb)
	{
		readCallback_ = std::move(cb);
	}

	void setWriteCallback(EventCallback cb)
	{
		writeCallback_ = std::move(cb);
	}

	void setCloseCallback(EventCallback cb)
	{
		closeCallback_ = std::move(cb);
	}

	void setErrorCallback(EventCallback cb)
	{
		errorCallback_ = std::move(cb);
	}

	// 将shared_ptr指向的对象与Channel绑定起来
	// 以防止资源在handleEvent中被销毁
	void tie(const std::shared_ptr<void>&);

	int fd() const { return fd_; }
	int events() const { return events_; }
	void set_revents(int revt) { revents_ = revt; }
	bool isNoneEvent() const { return events_ == kNoneEvent; }

	// 开启监听读事件
	void enableReading()
	{
		events_ |= kReadEvent;
		update();
	}

	// 关闭监听读事件
	void disableReading()
	{
		events_ &= ~kReadEvent;
		update();
	}

	void enableWriting()
	{
		events_ |= kWriteEvent;
		update();
	}

	void disableWriting()
	{
		events_ &= ~kWriteEvent;
		update();
	}

	void disableAll()
	{
		events_ = kNoneEvent;
		update();
	}

	bool isWriting() const { return events_ & kWriteEvent; }
	bool isReading() const { return events_ & kReadEvent; }

	// for Poller
	int index() const { return index_; }
	void set_index(int idx) { index_ = idx; }
	
	// for debug
	string reventsToString() const;
	string eventsToString() const;

	void doNotLogHup() { logHup_ = false; }

	EventLoop* ownerLoop() { return loop_; }
	void remove();

private:
	static string eventsToString(int fd, int ev);

	void update();
	void handleEventWithGuard(Timestamp receiveTime);

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;
	
	EventLoop* loop_;		// Channel所属的事件循环
	const int fd_;			// Channel所监视的文件描述符
	int events_;				// 监听的事件
	int revents_;				// 实际发生的事件(epoll或poll系统调用的返回值)
	int index_;					// 被Poller所使用
	bool logHup_;

	std::weak_ptr<void> tie_;
	bool tied_;
	bool eventHandling_;
	bool addedToLoop_;

	ReadEventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback closeCallback_;
	EventCallback errorCallback_;
};

};	// namespace net
};	// namespace zing

#endif