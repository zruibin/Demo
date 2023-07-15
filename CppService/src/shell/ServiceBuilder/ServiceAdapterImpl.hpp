/*
 * ServiceAdapter.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEADAPTERIMPL_H
#define SERVICEADAPTERIMPL_H

#include <iostream>
#include "ServiceAdapterInterface.h"
#include "LoggerInterface.h"

class ServiceAdapterImpl: public ServiceAdapterInterface
{
public:
    ServiceAdapterImpl() = default;
    ~ServiceAdapterImpl() = default;

public:
    void print() override {
        Log(DEBUG) << "ServiceAdapterImpl::print";
    };
};


#endif /* !SERVICEADAPTERIMPL_H */
