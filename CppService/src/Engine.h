/*
 * Engine.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <unordered_map>
#include "BaseEngine.h"
#include "ComponentFactory.h"

class Engine: public BaseEngine
{
public:
    Engine();
    virtual ~Engine();

protected:
    virtual std::shared_ptr<BaseService> GetServiceByName(const std::string&) override;

public:
    void Init(void) override;
    void Destory(void) override;
    void UpdateService(std::shared_ptr<BaseServiceBuilder>) override;

private:
    std::shared_ptr<std::unordered_map<std::string, 
                                       std::shared_ptr<BaseService>>> serviceMap_;
    std::shared_ptr<ComponentFactory> componentFactory_;
};


#endif /* !ENGINE_H */