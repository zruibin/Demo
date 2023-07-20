/*
 * BaseServiceFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASESERVICEFACTORY_H
#define BASESERVICEFACTORY_H

class BaseServiceBuilder;

class BaseServiceFactory
{
public:
    virtual ~BaseServiceFactory() = default;
    
public:
    virtual void ConstructBuilders() = 0;
    virtual std::shared_ptr<BaseServiceBuilder> GetBuilder(const std::string&) = 0;
    virtual void UpdateBuilder(std::shared_ptr<BaseServiceBuilder>) = 0;
};

#endif /* !BASESERVICEFACTORY_H */
