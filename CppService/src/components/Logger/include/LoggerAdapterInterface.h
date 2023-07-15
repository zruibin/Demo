/*
 * LoggerAdapterInterface.h
 *
 * Created by ruibin.chow on 2023/07/15.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef LOGGERADAPTERINTERFACE_H
#define LOGGERADAPTERINTERFACE_H

#include "BaseServiceAdapter.h"

class LoggerAdapterInterface : public BaseServiceAdapter
{
public:
    virtual ~LoggerAdapterInterface() = default;
};

#endif /* !LOGGERADAPTERINTERFACE_H */
