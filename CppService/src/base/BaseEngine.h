/*
 * BaseEngine.h
 *
 * Created by Ruibin.Chow on 2023/07/10.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASEENGINE_H
#define BASEENGINE_H

#include <string>
#include <memory>

#define Protocol(name) #name

class BaseService;
class BaseServiceBuilder;

class BaseEngine: public std::enable_shared_from_this<BaseEngine>
{
public:
    virtual ~BaseEngine() = default;

public:
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseService, T>>>
    std::shared_ptr<T> GetService(const std::string& name) {
        return std::dynamic_pointer_cast<T>(this->GetServiceByName(name));
    }
    
public:
    virtual void Init(void) = 0;
    virtual void Destory(void) = 0;
    virtual void UpdateService(std::shared_ptr<BaseServiceBuilder>) = 0;
    
protected:
    virtual std::shared_ptr<BaseService> GetServiceByName(const std::string&) = 0;
};

#endif /* !BASEENGINE_H */
