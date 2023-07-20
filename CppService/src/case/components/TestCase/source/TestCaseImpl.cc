/*
 * TestCaseImpl.h
 *
 * Created by ruibin.chow on 2023/07/20.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#include "TestCaseImpl.h"
#include <memory>
#include "BaseCaseEngine.h"
#include "BaseServiceEngine.h"
#include "LoggerInterface.h"
#include "ServiceInterface.h"

void TestCaseImpl::Init() {
    Log(DEBUG) << " TestCaseImpl::Init: " << this;
}

void TestCaseImpl::Destory() {
    Log(DEBUG) << " TestCaseImpl::Destory: " << this;
}

void TestCaseImpl::SetAdapter(const std::shared_ptr<BaseCaseAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<TestCaseAdapterInterface>();
}

std::string TestCaseImpl::Description() {
    
    auto service = adapter_->GetServiceEngine()->GetService<ServiceInterface>(Protocol(ServiceInterface));
    if (service) {
        Log(VERBOSE) << service->GetTestString();
    }
    
    return "TestCaseImpl";
}
