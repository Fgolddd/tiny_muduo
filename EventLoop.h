#pragma once

#include "CurrentThread.h"
#include "Timestamp.h"
#include "noncopyable.h"

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

class Channel;
class Poller;

// 事件循环类 主要包含Channel Poller（epoll的抽象）
class EventLoop {
public:
    using Functor = std::function<void()>;
    EventLoop();
    ~EventLoop();

    void loop();
    void quit();

    Timestamp pollReturnTime() const { return pollReturnTime_; }

    // 在当前loop中执行
    void runInLoop(Functor cb);
    // 把cb放入队列，唤醒loop所在的线程，执行
    void queueInLoop(Functor cb);

    // 唤醒loop所在的线程
    void wakeup();

    void updateChannel(Channel *channel);
    void removeChannel(Channel *channel);
    bool hasChannel(Channel *channel);
    bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

private:
    void handleRead();
    void doPendingFunctors();

    using ChannelList = std::vector<Channel *>;
    std::atomic_bool looping_;
    std::atomic_bool quit_;

    const pid_t threadId_;
    Timestamp pollReturnTime_;
    std::unique_ptr<Poller> poller_;
    // 当mainloop获取一个新用户的channel，通过wakeupfd唤醒subloop
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;

    ChannelList activeChannels_;
    Channel *currentActiveChannel_;

    // 标识当前loop是否有需要执行的回调函数
    std::atomic_bool callingPendingFunctors_;
    // 回调函数队列
    std::vector<Functor> pendingFunctors_;
    // 保护vector的线程安全操作
    std::mutex mutex_;
};