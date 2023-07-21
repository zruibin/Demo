/*
 * ServiceEngine.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEENGINE_H
#define SERVICEENGINE_H

#include <unordered_map>
#include "BaseServiceEngine.h"

class ServiceEngine : public BaseServiceEngine
{
public:
    ServiceEngine();
    virtual ~ServiceEngine();

protected:
    std::shared_ptr<BaseService> GetServiceByName(const char*) override;
    std::shared_ptr<BaseService> CreateServiceByName(const char*) override;

public:
    void Init(void) override;
    void Destory(void) override;
    void UpdateService(std::shared_ptr<BaseServiceBuilder>) override;

private:
    std::shared_ptr<std::unordered_map<const char*, 
                                       std::shared_ptr<BaseService>>> serviceMap_;
};


#endif /* !SERVICEENGINE_H */
