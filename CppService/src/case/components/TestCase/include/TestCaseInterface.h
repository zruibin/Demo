/*
 * TestCaseInterface.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef TESTCASEINTERFACE_H
#define TESTCASEINTERFACE_H

#include <memory>
#include "BaseCase.h"

class TestCaseInterface : public BaseCase
{
public:
    virtual ~TestCaseInterface() = default;
};

#endif /* !TESTCASEINTERFACE_H */