#pragma once

#include "noncopyable.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

class EventLoop;
class EventLoopThread;

class EventLoopThreadPoll : noncopyable {
public:
    using ThreadInitCallback = std::function<void(EventLoop *)>;
    EventLoopThreadPoll(EventLoop *baseLoop, const std::string &nameArg);
    ~EventLoopThreadPoll();

    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback &cb = ThreadInitCallback());

    // 轮询方式分配channel
    EventLoop *getNextLoop();
    std::vector<EventLoop *> getAllLoops();
    bool started() const { return started_; }
    const std::string name() const { return name_; }

private:
    EventLoop *baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop *> loops_;
};