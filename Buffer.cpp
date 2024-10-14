#include "Buffer.h"
#include <errno.h>
#include <sys/uio.h>
#include <unistd.h>
ssize_t Buffer::readFd(int fd, int *saveErrno) {
    // 栈上内存空间
    char extrabuf[65536] = {0};

    // 缓冲区剩余可写大小
    const size_t writable = writableBytes();

    // 先把数据完全写在缓冲区，若超过缓冲区大小，
    // 则多余的数据写在栈上，由extrabuf记录, 把缓冲区扩容后再写回
    struct iovec vec[2];
    vec[0].iov_base = begin() + writerIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extrabuf;
    vec[1].iov_len = sizeof extrabuf;

    const int iovecCount = (writable < sizeof extrabuf) ? 2 : 1;
    const ssize_t n = ::readv(fd, vec, iovecCount);
    if (n < 0) {
        *saveErrno = errno;
    } else if (n <= writable) {
        writerIndex_ += n;
    } else {
        writerIndex_ = buffer_.size();
        append(extrabuf, n - writable);
    }

    return n;
}

ssize_t Buffer::writeFd(int fd, int *saveErrno) {
    ssize_t n = ::write(fd, peek(), readableBytes());
    if (n < 0) {
        *saveErrno = errno;
    }
    return n;
}