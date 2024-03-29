/*
 * BaseBuilder.h
 *
 * Created by Ruibin.Chow on 2023/07/10.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASEBUILDER_H
#define BASEBUILDER_H

#include <memory>

class BaseService;
class BaseServiceEngine;

class BaseServiceBuilder : public std::enable_shared_from_this<BaseServiceBuilder>
{
public:
    virtual ~BaseServiceBuilder() = default;
    
public:
    virtual std::shared_ptr<BaseService> BuildService() = 0;
    virtual const char* GetServiceName() = 0;
    virtual const char* GetBuilderName() = 0;
    virtual void SetEngine(std::shared_ptr<BaseServiceEngine> engine) {
        engine_ = engine;
    }
    
protected:
    std::weak_ptr<BaseServiceEngine> engine_;
};


#endif /* !BASEBUILDER_H */
