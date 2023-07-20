/*
 * TestCaseImpl.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef TESTCASEIMPL_H
#define TESTCASEIMPL_H

#include "TestCaseInterface.h"
#include "TestCaseAdapterInterface.h"

class TestCaseImpl : public TestCaseInterface
{
public:
    explicit TestCaseImpl() = default;
    virtual ~TestCaseImpl() = default;
    
public:
    void Init() override;
    void Destory() override;
    void SetAdapter(const std::shared_ptr<BaseCaseAdapter>) override;
    std::string Description() override;
    
private:
    std::shared_ptr<TestCaseAdapterInterface> adapter_;
};

#endif /* !TESTCASEIMPL_H */