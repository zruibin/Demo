/*
 * BaseCaseAdapter.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECASEADAPTER_H
#define BASECASEADAPTER_H

#include <memory>

class BaseCaseEngine;
class BaseServiceEngine;

class BaseCaseAdapter : public std::enable_shared_from_this<BaseCaseAdapter>
{
public:
    virtual ~BaseCaseAdapter() = default;

public:
    virtual std::shared_ptr<BaseCaseEngine> GetCaseEngine() {
        if (caseEngine_.expired()) {
            return nullptr;
        }
        return caseEngine_.lock();
    }
    
    virtual void SetCaseEngine(std::shared_ptr<BaseCaseEngine> caseEngine) {
        caseEngine_ = caseEngine;
    }
    
    virtual std::shared_ptr<BaseServiceEngine> GetServiceEngine() {
        if (serviceEngine_.expired()) {
            return nullptr;
        }
        return serviceEngine_.lock();
    }
    
    virtual void SetServiceEngine(std::shared_ptr<BaseServiceEngine> serviceEngine) {
        serviceEngine_ = serviceEngine;
    }
    
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseCaseAdapter, T>>>
    std::shared_ptr<T> GetCastAdapter() {
        return std::dynamic_pointer_cast<T>(this->shared_from_this());
    }

protected:
    std::weak_ptr<BaseCaseEngine> caseEngine_;
    std::weak_ptr<BaseServiceEngine> serviceEngine_;
};

#endif /* !BASECASEADAPTER_H */
