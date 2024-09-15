#include "Socket.h"
#include "InetAddress.h"
#include "Logger.h"

#include <cstring>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
Socket::~Socket() { ::close(sockfd_); }

void Socket::bindAddress(const InetAddress &localaddr) {
    if (0 != ::bind(sockfd_, (sockaddr *)localaddr.getSockAddr(),
                    sizeof(sockaddr_in))) {
        LOG_FATAL("bind sockfd:%d failed\n", sockfd_);
    }
}
void Socket::listen() {
    if (0 != ::listen(sockfd_, 1024)) {
        LOG_FATAL("listen sockfd:%d failed\n", sockfd_);
    }
}
int Socket::accept(InetAddress *peeraddr) {
    sockaddr_in addr;
    socklen_t len;
    memset(&addr, 0, sizeof addr);
    int connfd = ::accept(sockfd_, (sockaddr *)&addr, &len);
    if (connfd >= 0) {
        peeraddr->setSockAddr(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    if (::shutdown(sockfd_, SHUT_WR) < 0) {
        LOG_ERROR("Socket::shutdownWrite error!");
    }
}

int Socket::createNonblockingOrDie() {
    int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
                          IPPROTO_TCP);
    if (sockfd < 0) {
        LOG_FATAL("listen socket create failed!\n");
    }
    return sockfd;
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof optval);
}
void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);
}
void Socket::setReusePort(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, SO_REUSEPORT, &optval, sizeof optval);
}
void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, SO_KEEPALIVE, &optval, sizeof optval);
}