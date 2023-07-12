/*
 * ServiceImpl.h
 *
 * Created by Ruibin.Chow on 2023/06/30.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef SERVICEIMPL_H
#define SERVICEIMPL_H

#include "ServiceInterface.h"
#include "ServiceAdapterInterface.h"

class ServiceImpl : public ServiceInterface
{
public:
    explicit ServiceImpl();
    virtual ~ServiceImpl();
    
public:
    void Init() override;
    void Destory() override;
    void SetAdapter(const std::shared_ptr<BaseServiceAdapter>) override;
    void Description() override;
    
public:
    void Test() override;
    std::string GetTestString() override;
    
private:
    std::shared_ptr<ServiceAdapterInterface> adapter_;
};


#endif /* !SERVICEIMPL_H */
