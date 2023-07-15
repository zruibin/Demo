/*
 * LoggerBuilder.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef LOGGERBUILDER_H
#define LOGGERBUILDER_H

#include "BaseServiceBuilder.h"
#include "BaseEngine.h"
#include "BaseService.h"
#include "LoggerImpl.h"
#include "LoggerAdapterImpl.hpp"

class LoggerBuilder: public BaseServiceBuilder
{
public:
    LoggerBuilder() = default;
    virtual ~LoggerBuilder() = default;

public:
    std::shared_ptr<BaseService> BuildService() override {
        std::shared_ptr<BaseServiceAdapter> adapter = std::make_shared<LoggerAdapterImpl>();
        adapter->SetEngine(engine_.lock());
        std::shared_ptr<LoggerInterface> impl = std::make_shared<LoggerImpl>();
        impl->SetAdapter(adapter);
        return impl;
    };

    std::string GetServiceName() override {
        return "LoggerInterface";
    };
    
    std::string GetBuilderName() override {
        return "LoggerBuilder";
    };
};

#endif /* !LOGGERBUILDER_H */
