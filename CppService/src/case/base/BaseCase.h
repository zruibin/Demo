/*
 * BaseCase.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECASE_H
#define BASECASE_H

#include "BaseCaseAdapter.h"

class BaseCase : public std::enable_shared_from_this<BaseCase>
{
public:
    virtual ~BaseCase() = default;

public:
    virtual void SetAdapter(const std::shared_ptr<BaseCaseAdapter>) = 0;
    virtual void Init(void) = 0;
    virtual void Destory(void) = 0;
    
public:
    virtual const char* Description() { return "";};
};


#endif /* !BASECASE_H */
