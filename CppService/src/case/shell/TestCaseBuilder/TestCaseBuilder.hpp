/*
 * TestCaseBuilder.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef TESTCASEBUILDER_H
#define TESTCASEBUILDER_H

#include "BaseCaseBuilder.h"
#include "BaseCaseEngine.h"
#include "BaseCase.h"
#include "TestCaseImpl.h"
#include "TestCaseAdapterImpl.hpp"

class TestCaseBuilder : public BaseCaseBuilder
{
public:
    TestCaseBuilder() = default;
    virtual ~TestCaseBuilder() = default;

public:
    std::shared_ptr<BaseCase> BuildCase() override {
        std::shared_ptr<BaseCaseAdapter> adapter = std::make_shared<TestCaseAdapterImpl>();
        adapter->SetServiceEngine(serviceEngine_.lock());
        adapter->SetCaseEngine(caseEngine_.lock());
        std::shared_ptr<TestCaseInterface> impl = std::make_shared<TestCaseImpl>();
        impl->SetAdapter(adapter);
        return impl;
    };

    const char* GetCaseName() override {
        return "TestCaseInterface";
    };
    
    const char* GetBuilderName() override {
        return "TestCaseBuilder";
    };
};

#endif /* !TESTCASEBUILDER_H */
