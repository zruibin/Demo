/*
 * TestCaseAdapterImpl.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef TESTCASEADAPTERIMPL_H
#define TESTCASEADAPTERIMPL_H

#include "TestCaseAdapterInterface.h"

class TestCaseAdapterImpl : public TestCaseAdapterInterface
{
public:
    TestCaseAdapterImpl() = default;
    ~TestCaseAdapterImpl() = default;
};

#endif /* !TESTCASEADAPTERIMPL_H */
