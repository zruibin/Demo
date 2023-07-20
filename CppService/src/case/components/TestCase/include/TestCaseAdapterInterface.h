/*
 * TestCaseAdapterInterface.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef TESTCASEADAPTERINTERFACE_H
#define TESTCASEADAPTERINTERFACE_H

#include "BaseCaseAdapter.h"

class TestCaseAdapterInterface : public BaseCaseAdapter
{
public:
    virtual ~TestCaseAdapterInterface() = default;
};

#endif /* !TESTCASEADAPTERINTERFACE_H */