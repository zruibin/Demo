/*
 * ServiceBuilder.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEBUILDER_H
#define SERVICEBUILDER_H

#include "BaseServiceBuilder.h"
#include "BaseEngine.h"
#include "BaseService.h"
#include "ServiceImpl.h"
#include "ServiceAdapterImpl.hpp"

class ServiceBuilder: public BaseServiceBuilder
{
public:
    ServiceBuilder() = default;
    virtual ~ServiceBuilder() = default;

public:
    std::shared_ptr<BaseService> BuildService() override {
        std::shared_ptr<ServiceAdapterImpl> adapter = std::make_shared<ServiceAdapterImpl>();
        adapter->SetEngine(engine_.lock());
        std::shared_ptr<ServiceInterface> serviceImpl = std::make_shared<ServiceImpl>();
        serviceImpl->SetAdapter(adapter);
        return serviceImpl;
    };

    std::string GetServiceName() override {
        return "ServiceInterface";
    };
    
    std::string GetBuilderName() override {
        return "ServiceBuilder";
    };
};

#endif /* !SERVICEBUILDER_H */
