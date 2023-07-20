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
class BaseServiceFactory;

class BaseEngine : public std::enable_shared_from_this<BaseEngine>
{
public:
    virtual ~BaseEngine() = default;

public:
    /**
     此方法获取到的Service生命周期跟随Engine，Service的Init与Destory由Engine操控！
     */
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseService, T>>>
    std::shared_ptr<T> GetService(const std::string& name) {
        std::shared_ptr<BaseService> baseService = this->GetServiceByName(name);
        if (baseService != nullptr) {
            return std::dynamic_pointer_cast<T>(baseService);
        }
        return nullptr;
    }
    
    /**
     此方法获取到的Service生命周期由调用者操控，Service的Init与Destory必须由调用者显式调用！
     */
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseService, T>>>
    std::shared_ptr<T> CreateService(const std::string& name) {
        std::shared_ptr<BaseService> baseService = this->CreateServiceByName(name);
        if (baseService != nullptr) {
            return std::dynamic_pointer_cast<T>(baseService);
        }
        return nullptr;
    }
    
public:
    virtual void Init(void) = 0;
    virtual void Destory(void) = 0;
    virtual void UpdateService(std::shared_ptr<BaseServiceBuilder>) = 0;
    
protected:
    virtual std::shared_ptr<BaseService> GetServiceByName(const std::string&) = 0;
    virtual std::shared_ptr<BaseService> CreateServiceByName(const std::string&) = 0;
    std::shared_ptr<BaseServiceFactory> componentFactory_;
};

#endif /* !BASEENGINE_H */
