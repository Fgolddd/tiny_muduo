#pragma once

#include "Timestamp.h"
#include "noncopyable.h"
#include <functional>
#include <memory>

class EventLoop;

// 封装了sockfd和其感兴趣的event，如EPOLLIN、EPOLLOUT事件
// 还绑定了poller返回的具体事件
class Channel : noncopyable {
public:
    using EventCallback = std::function<void()>;
    using ReadEventCallback = std::function<void(Timestamp)>;

    Channel(EventLoop *loop, int fd);
    ~Channel();
    void handleEvent(Timestamp receiveTime);

private:
    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop *loop_; // 事件循环
    const int fd_;    // fd,Poller监听的对象
    int events_;      // 注册fd感兴趣的事件
    int revents_;     // poller返回的具体事件
    int index_;

    std::weak_ptr<void> tie_;
    bool tied_;

    // 回调函数
    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};