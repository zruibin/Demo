/*
 * ServiceEngine.h
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEENGINE_H
#define SERVICEENGINE_H

#include <unordered_map>
#include "BaseEngine.h"

class ServiceEngine : public BaseEngine
{
public:
    ServiceEngine();
    virtual ~ServiceEngine();

protected:
    std::shared_ptr<BaseService> GetServiceByName(const std::string&) override;
    std::shared_ptr<BaseService> CreateServiceByName(const std::string&) override;

public:
    void Init(void) override;
    void Destory(void) override;
    void UpdateService(std::shared_ptr<BaseServiceBuilder>) override;

private:
    std::shared_ptr<std::unordered_map<std::string, 
                                       std::shared_ptr<BaseService>>> serviceMap_;
};


#endif /* !SERVICEENGINE_H */
