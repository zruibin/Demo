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
#include "BaseEngine.h"

ServiceImpl::ServiceImpl() {
    std::cout << "ServiceImpl::ServiceImpl: " << this << std::endl;
}

ServiceImpl::~ServiceImpl() {
    std::cout << "ServiceImpl::~ServiceImpl: " << this << std::endl;
}

void ServiceImpl::Init() {
    std::cout << "ServiceImpl::Init." << std::endl;
}

void ServiceImpl::Destory() {
    std::cout << "ServiceImpl::Destory." << std::endl;
}

void ServiceImpl::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<ServiceAdapterInterface>();
}

void ServiceImpl::Description() {
    std::cout << "ServiceImpl::Description." << std::endl;
    adapter_->print();
}

void ServiceImpl::Test() {
    std::cout << "ServiceImpl::test." << std::endl;
}

std::string ServiceImpl::GetTestString() {
    return "TestString...";
}
