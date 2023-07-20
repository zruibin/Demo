/*
 * BaseCaseEngine.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECASEENGINE_H
#define BASECASEENGINE_H


#include <string>
#include <memory>

#define Protocol(name) #name

class BaseCase;
class BaseCaseBuilder;
class BaseCaseFactory;
class BaseServiceEngine;

class BaseCaseEngine : public std::enable_shared_from_this<BaseCaseEngine>
{
public:
    virtual ~BaseCaseEngine() = default;

public:
    /**
     此方法获取到的Case生命周期跟随Engine，Case的Init与Destory由Engine操控！
     */
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseCase, T>>>
    std::shared_ptr<T> GetCase(const std::string& name) {
        std::shared_ptr<BaseCase> baseCase = this->GetCaseByName(name);
        if (baseCase != nullptr) {
            return std::dynamic_pointer_cast<T>(baseCase);
        }
        return nullptr;
    }
    
    /**
     此方法获取到的Case生命周期由调用者操控，Case的Init与Destory必须由调用者显式调用！
     */
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseCase, T>>>
    std::shared_ptr<T> CreateCase(const std::string& name) {
        std::shared_ptr<BaseCase> baseCase = this->CreateCaseByName(name);
        if (baseCase != nullptr) {
            return std::dynamic_pointer_cast<T>(baseCase);
        }
        return nullptr;
    }
    
public:
    virtual void Init(void) = 0;
    virtual void Destory(void) = 0;
    virtual void UpdateCase(std::shared_ptr<BaseCaseBuilder>) = 0;
    
public:
    void SetServiceEngine(std::shared_ptr<BaseServiceEngine> serviceEngine) {
        serviceEngine_ = serviceEngine;
    };
    
protected:
    virtual std::shared_ptr<BaseCase> GetCaseByName(const std::string&) = 0;
    virtual std::shared_ptr<BaseCase> CreateCaseByName(const std::string&) = 0;
    std::shared_ptr<BaseCaseFactory> caseFactory_;
    std::weak_ptr<BaseServiceEngine> serviceEngine_;
};


#endif /* !BASECASEENGINE_H */
