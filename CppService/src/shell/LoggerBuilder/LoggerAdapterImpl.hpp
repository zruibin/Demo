/*
 * LoggerAdapterImpl.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef LOGGERADAPTERIMPL_H
#define LOGGERADAPTERIMPL_H

#include "LoggerAdapterInterface.h"

class LoggerAdapterImpl : public LoggerAdapterInterface
{
public:
    LoggerAdapterImpl() = default;
    ~LoggerAdapterImpl() = default;
};

#endif /* !LOGGERADAPTERIMPL_H */
