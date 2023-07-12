/*
 * ServiceAdapter.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEADAPTERINTERFACE_H
#define SERVICEADAPTERINTERFACE_H

#include "BaseServiceAdapter.h"
#include <string>

class ServiceAdapterInterface: public BaseServiceAdapter
{
public:
    virtual ~ServiceAdapterInterface() = default;

public:
    virtual void print() = 0;
};


#endif /* !SERVICEADAPTER_H */
