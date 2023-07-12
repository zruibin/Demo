/*
 * BaseService.h
 *
 * Created by Ruibin.Chow on 2023/07/10.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASESERVICE_H
#define BASESERVICE_H

#include "BaseServiceAdapter.h"

class BaseService: public std::enable_shared_from_this<BaseService>
{
public:
    virtual ~BaseService() = default;

public:
    virtual void SetAdapter(const std::shared_ptr<BaseServiceAdapter>) = 0;
    virtual void Init(void) = 0;
    virtual void Destory(void) = 0;
    
public:
    virtual void Description() = 0;
};


#endif /* !BASESERVICE_H */
