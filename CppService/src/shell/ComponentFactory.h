/*
 * ComponentFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef COMPONENTFACTORY_H
#define COMPONENTFACTORY_H

#include <memory>
#include <vector>

class BaseServiceBuilder;
class BaseEngine;

class ComponentFactory final {
    
public:
    explicit ComponentFactory();
    
public:
    void ConstructBuilders();
    using SharedBuilders = std::shared_ptr<std::vector<std::shared_ptr<BaseServiceBuilder>>>;
    SharedBuilders GetBuilders() { return builders_;};
    
private:
    SharedBuilders builders_;
};


#endif /* !COMPONENTFACTORY_H */
