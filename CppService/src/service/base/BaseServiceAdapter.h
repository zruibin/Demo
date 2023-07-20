/*
 * BaseServiceAdapter.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASESERVICEADAPTER_H
#define BASESERVICEADAPTER_H

#include <memory>

class BaseServiceEngine;

class BaseServiceAdapter : public std::enable_shared_from_this<BaseServiceAdapter>
{
public:
    virtual ~BaseServiceAdapter() = default;

public:
    virtual std::shared_ptr<BaseServiceEngine> GetEngine() {
        if (engine_.expired()) {
            return nullptr;
        }
        return engine_.lock();
    }
    
    virtual void SetEngine(std::shared_ptr<BaseServiceEngine> engine) {
        engine_ = engine;
    }
    
    template <class T, class = std::enable_if_t<std::is_base_of_v<BaseServiceAdapter, T>>>
    std::shared_ptr<T> GetCastAdapter() {
        return std::dynamic_pointer_cast<T>(this->shared_from_this());
    }

protected:
    std::weak_ptr<BaseServiceEngine> engine_;
};

#endif /* !BASESERVICEADAPTER_H */
