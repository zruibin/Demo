/*
 * ServiceInterface.h
 *
 * Created by Ruibin.Chow on 2023/06/30.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEINTERFACE_H
#define SERVICEINTERFACE_H

#include <memory>
#include "BaseService.h"

class ServiceInterface : public BaseService
{
public:
    virtual ~ServiceInterface() = default;
    
public:
    virtual void Test() = 0;
    virtual std::string GetTestString() = 0;
};

#endif /* !SERVICEINTERFACE_H */
