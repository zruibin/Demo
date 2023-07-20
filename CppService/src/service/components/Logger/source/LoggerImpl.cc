/*
 * LoggerImpl.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#include "LoggerImpl.h"
#include <iostream>
#include <memory>
#include "BaseEngine.h"

void LoggerImpl::Init() {
    Log(DEBUG) << "LoggerImpl::Init.";
}

void LoggerImpl::Destory() {
    Log(DEBUG) << "LoggerImpl::Destory.";
}

void LoggerImpl::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<LoggerAdapterInterface>();
}

std::string LoggerImpl::Description() {
    return "LoggerImpl";
}

void LoggerImpl::SetMinSeverity(LoggingSeverity minLevel) {
    LoggerManager::GetInstance().SetMinSeverity(minLevel);
}

void LoggerImpl::InjectLoggerHanlder(LoggerHandler handler) {
    if (handler) {
        LoggerManager::GetInstance().SetLoggerHander(handler);
    }
}

static LoggingSeverity minWriteLevel = LoggingSeverity::INFO;

LoggerManager& LoggerManager::GetInstance() {
    static LoggerManager *instance = nullptr;
    static std::once_flag flag;
    std::call_once(flag, []() {
        if (instance == nullptr) {
            instance = new LoggerManager();
        }
    });
    return *instance;
}

void LoggerManager::SetMinSeverity(LoggingSeverity minLevel) {
    minWriteLevel = minLevel;
}

void LoggerManager::SetLoggerHander(LoggerHandler handler) {
    if (handler) {
        handler_ = handler;
    }
}

void LoggerManager::Write(LoggingSeverity severity, const char* string) {
    if (handler_) {
        handler_(severity, string);
    }
}

static const char* loggingSeverityCover(LoggingSeverity severity) {
    static const char* severityList[] = {
        [static_cast<int>(LoggingSeverity::VERBOSE)] = "[V]",
        [static_cast<int>(LoggingSeverity::INFO)] = "[I]",
        [static_cast<int>(LoggingSeverity::DEBUG)] = "[D]",
        [static_cast<int>(LoggingSeverity::WARNING)] = "[W]",
        [static_cast<int>(LoggingSeverity::ERROR)] = "[E]",
        [static_cast<int>(LoggingSeverity::NONE)] = "[N]",
    };
    
    return severityList[static_cast<int>(severity)];
}

LogMessage::LogMessage(const char* file, int line, LoggingSeverity severity, bool origin)
        : stringBuffer_(new std::string), origin_(origin) {
    severity_ = severity;
    std::string fileStr(file);
//    stringBuffer_->append(util::get_current_time_string());
//    stringBuffer_->append(" [");
//    stringBuffer_->append(std::to_string(platform::thread_get_current_id()));
//    stringBuffer_->append("]");
    if (!origin_) {
        stringBuffer_->append("[");
        stringBuffer_->append(fileStr.substr(fileStr.find_last_of("/")+1));
        stringBuffer_->append(":");
        stringBuffer_->append(std::to_string(line));
        stringBuffer_->append("]");
        stringBuffer_->append(loggingSeverityCover(severity));
        stringBuffer_->append(" ");
    }
}

LogMessage::LogMessage(const char* file, int line, LoggingSeverity severity)
        : LogMessage(file, line, severity, false) {}

LogMessage::~LogMessage() {
    if (!origin_) {
        stringBuffer_->append("\n");
    }
//    std::cout << stringBuffer_->c_str();
    if (severity_ >= minWriteLevel) {
        LoggerManager::GetInstance().Write(severity_, stringBuffer_->c_str());
    }
    delete stringBuffer_;
}

