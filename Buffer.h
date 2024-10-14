#pragma

#include <algorithm>
#include <cstddef>
#include <string>
#include <vector>

class Buffer {
public:
    static const size_t kCheapPrepend = 8;
    static const size_t kInitialSize = 1024;
    explicit Buffer(size_t initialSize = kInitialSize)
        : buffer_(kCheapPrepend + initialSize), readerIndex_(kCheapPrepend),
          writerIndex_(kCheapPrepend) {}
    ~Buffer();

    size_t readableBytes() const { return writerIndex_ - readerIndex_; }
    size_t writableBytes() const { return buffer_.size() - writerIndex_; }
    size_t prependableBytes() const { return readerIndex_; }

    // 可读数据的起始地址
    const char *peek() const { return begin() + readerIndex_; }
    void retrieve(size_t len) {
        if (len < readableBytes()) {
            // 数据还没读完
            readerIndex_ += len;
        } else {
            // 数据读完，清空缓冲区
            retrieveAll();
        }
    }
    // 清空缓冲区
    void retrieveAll() { readerIndex_ = writerIndex_ = kCheapPrepend; }

    // 转成string类型返回
    std::string retrieveAsString(size_t len) {
        std::string result(peek(), len);
        // 复位
        retrieve(len);
        return result;
    }
    std::string retrieveAllAsString() {
        return retrieveAsString(readableBytes());
    }

    void ensureWritable(size_t len) {
        // 可写缓冲区不够写，扩容
        if (writableBytes() < len) {
            makeSpace(len);
        }
    }

    // 把内存上的数据添加到缓冲区当中
    void append(const char *data, size_t len) {
        ensureWritable(len);
        std::copy(data, data + len, begin() + writerIndex_);
        writerIndex_ += len;
    }

    // 从fd上读取数据
    ssize_t readFd(int fd, int *saveErrno);

    // 通过fd发送数据
    ssize_t writeFd(int fd, int *saveErrno);

private:
    char *begin() { return &*buffer_.begin(); }
    const char *begin() const { return &*buffer_.begin(); }
    void makeSpace(size_t len) {
        if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
            buffer_.resize(writerIndex_ + len);
        } else {
            // 移动数据,让可写缓冲区足够写入
            size_t readable = readableBytes();
            std::copy(begin() + readerIndex_, begin() + writerIndex_,
                      begin() + kCheapPrepend);
            readerIndex_ = kCheapPrepend;
            writerIndex_ = readerIndex_ + readable;
        }
    }
    std::vector<char> buffer_;
    size_t readerIndex_;
    size_t writerIndex_;
};