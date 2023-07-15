/*
 * LoggerInterface.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

#include <memory>
#include <cstdio>
#include <string>
#include <sstream>
#include <functional>
#include "BaseService.h"

#define Log(severity) LogMessage(__FILE__, __LINE__, severity)
#define LogOrigin(severity) LogMessage(__FILE__, __LINE__, severity, true)

enum LoggingSeverity {
    NONE,
    VERBOSE,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
};

using LoggerHandler = std::function<void(LoggingSeverity, const char*)>;

class LoggerInterface: public BaseService
{
public:
    virtual ~LoggerInterface() = default;
public:
    virtual void SetMinSeverity(LoggingSeverity) = 0;
    virtual void InjectLoggerHanlder(LoggerHandler) = 0;
};


class LogMessage final
{
public:
    typedef LogMessage self;
public:
    LogMessage(const char* file, int line, LoggingSeverity severity, bool origin);
    LogMessage(const char* file, int line, LoggingSeverity severity);
    ~LogMessage();
    
public:
    self& operator<<(bool v) {
        stringBuffer_->append(v ? "1" : "0");
        return *this;
    }
    
    self& operator<<(short i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(unsigned short si) {
        stringBuffer_->append(std::to_string(si));
        return *this;
    }

    self& operator<<(int i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(unsigned int i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(unsigned long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(long long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }
    
    self& operator<<(unsigned long long i) {
        stringBuffer_->append(std::to_string(i));
        return *this;
    }

    self& operator<<(float v) {
        *this << static_cast<double>(v);
        return *this;
    }
    
    self& operator<<(double d) {
        stringBuffer_->append(std::to_string(d));
        return *this;
    }
    
    self& operator<<(char v) {
        *this << static_cast<short>(v);
        return *this;
    }
    
    self& operator<<(const char* charStr) {
        stringBuffer_->append(charStr);
        return *this;
    }
    
    self& operator<<(const unsigned char* str) {
        return operator<<(reinterpret_cast<const char*>(str));
    }
    
    self& operator<<(const std::string& str) {
        stringBuffer_->append(str);
        return *this;
    }
    
    self& operator<<(const std::string_view str) {
        stringBuffer_->append(str.data(), str.length());
        return *this;
    }

    self& operator<<(void* pVoid) {
        std::ostringstream oss;
        oss << pVoid;
        stringBuffer_->append(oss.str());
        return *this;
    }
    
    self& operator<<(const void* pConstVoid) {
        std::ostringstream oss;
        oss << pConstVoid;
        stringBuffer_->append(oss.str());
        return *this;
    }

private:
    std::string* stringBuffer_;
    LoggingSeverity severity_;
    bool origin_ = false;
};


#endif /* !LOGGERINTERFACE_H */
