/*
 * AudioDectImpl.h
 *
 * Created by ruibin.chow on 2023/07/12.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#include "AudioDectImpl.h"
#include <iostream>
#include <memory>
#include "BaseEngine.h"
#include "ServiceInterface.h"

void AudioDectImpl::Init() {
    std::cout << "AudioDectImpl::Init." << std::endl;
}

void AudioDectImpl::Destory() {
    std::cout << "AudioDectImpl::Destory." << std::endl;
}

void AudioDectImpl::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<AudioDectAdapterInterface>();
}

void AudioDectImpl::Description() {
    std::cout << "AudioDectImpl::Description." << std::endl;
    
    std::cout << "**********************************************************" << std::endl;
    {
        std::shared_ptr<ServiceInterface> tmpService = adapter_->GetEngine()->CreateService<ServiceInterface>(Protocol(ServiceInterface));
        if (tmpService != nullptr) {
            tmpService->Init();
            tmpService->Description();
            tmpService->Test();
            tmpService->Destory();
        }}
    std::cout << "**********************************************************" << std::endl;
    
    auto service = adapter_->GetEngine()->GetService<ServiceInterface>(Protocol(ServiceInterface));
    std::cout << "AudioDectImpl::Description->" << service->GetTestString() << std::endl;
}
