#pragma once

#include <string>

#include "noncopyable.h"

#define LOG_INFO(logmsgFormat, ...)                                            \
    do {                                                                       \
        Logger &logger = Logger::instance();                                   \
        logger.setLogLevel(INFO);                                              \
        char buf[1024] = {0};                                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                      \
        logger.log(buf);                                                       \
    } while (0)

#define LOG_ERROR(logmsgFormat, ...)                                           \
    do {                                                                       \
        Logger &logger = Logger::instance();                                   \
        logger.setLogLevel(ERROR);                                             \
        char buf[1024] = {0};                                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                      \
        logger.log(buf);                                                       \
    } while (0)

#define LOG_FATAL(logmsgFormat, ...)                                           \
    do {                                                                       \
        Logger &logger = Logger::instance();                                   \
        logger.setLogLevel(FATAL);                                             \
        char buf[1024] = {0};                                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                      \
        logger.log(buf);                                                       \
    } while (0)
#ifdef MUDEBUG
#define LOG_DEBUG(logmsgFormat, ...)                                           \
    do {                                                                       \
        Logger &logger = Logger::instance();                                   \
        logger.setLogLevel(DEBUG);                                             \
        char buf[1024] = {0};                                                  \
        snprintf(buf, 1024, logmsgFormat, ##__VA_ARGS__);                      \
        logger.log(buf);                                                       \
    } while (0)
#else
#define LOG_DEBUG(logmsgFormat, ...)
#endif

// 定义日志级别 INFO ERROR FATAL DEBUG
enum LogLevel { INFO, ERROR, FATAL, DEBUG };

// 输出一个日志类 单例
class Logger : noncopyable {
  public:
    // 唯一的实例对象
    static Logger &instance();
    // 设置日志级别
    void setLogLevel(int level);
    // 写日志
    void log(std::string msg);

  private:
    int logLevel_;
    Logger() {}
};
