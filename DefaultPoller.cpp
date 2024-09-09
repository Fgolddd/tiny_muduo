#include "Poller.h"

// 默认选择epoll， 若需要可拓展poll等其他io复用方式
Poller *Poller::newDefaultPoller(EventLoop *loop) {
    // return new EPollPoller(loop);
    return nullptr;
}