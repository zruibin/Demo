/*
 * LoggerImpl.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef LOGGERIMPL_H
#define LOGGERIMPL_H

#include "LoggerInterface.h"
#include "LoggerAdapterInterface.h"

class LoggerImpl : public LoggerInterface
{
public:
    explicit LoggerImpl() = default;
    virtual ~LoggerImpl() = default;
    
public:
    void Init() override;
    void Destory() override;
    void SetAdapter(const std::shared_ptr<BaseServiceAdapter>) override;
    std::string Description() override;
    void SetMinSeverity(LoggingSeverity) override;
    void InjectLoggerHanlder(LoggerHandler) override;
    
private:
    std::shared_ptr<LoggerAdapterInterface> adapter_;
};

class LoggerManager final
{
public:
    static LoggerManager& GetInstance();
public:
    void SetMinSeverity(LoggingSeverity);
    void SetLoggerHander(LoggerHandler);
    void Write(LoggingSeverity, const char*);
private:
    LoggerHandler handler_;
};

#endif /* !LOGGERIMPL_H */
