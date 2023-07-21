/*
 *
 * ServiceImpl.cc
 *
 * Created by Ruibin.Chow on 2023/06/30.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "ServiceImpl.h"
#include <iostream>
#include <memory>
#include "BaseServiceEngine.h"
#include "LoggerInterface.h"

ServiceImpl::ServiceImpl() {
    Log(DEBUG) << "ServiceImpl::ServiceImpl: " << this;
}

ServiceImpl::~ServiceImpl() {
    Log(DEBUG) << "ServiceImpl::~ServiceImpl: " << this;
}

void ServiceImpl::Init() {
    Log(DEBUG) << "ServiceImpl::Init.";
}

void ServiceImpl::Destory() {
    Log(DEBUG)  << "ServiceImpl::Destory.";
}

void ServiceImpl::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<ServiceAdapterInterface>();
}

const char* ServiceImpl::Description() {
    Log(DEBUG) << "ServiceImpl::Description.";
    adapter_->print();
    return "ServiceImpl";
}

void ServiceImpl::Test() {
    Log(DEBUG) << "ServiceImpl::test.";
}

std::string ServiceImpl::GetTestString() {
    return "TestString...";
}
