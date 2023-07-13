/*
 * BaseComponentFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/13.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECOMPONENTFACTORY_H
#define BASECOMPONENTFACTORY_H

class BaseServiceBuilder;

class BaseComponentFactory {

public:
    virtual ~BaseComponentFactory() = default;
    
public:
    virtual void ConstructBuilders() = 0;
    virtual std::shared_ptr<BaseServiceBuilder> GetBuilder(const std::string&) = 0;
    virtual void UpdateBuilder(std::shared_ptr<BaseServiceBuilder>) = 0;
};

#endif /* !BASECOMPONENTFACTORY_H */
