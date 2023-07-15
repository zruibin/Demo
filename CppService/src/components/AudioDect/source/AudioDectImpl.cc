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
#include "LoggerInterface.h"

void AudioDectImpl::Init() {
    Log(DEBUG) << "AudioDectImpl::Init.";
}

void AudioDectImpl::Destory() {
    Log(DEBUG) << "AudioDectImpl::Destory.";
}

void AudioDectImpl::SetAdapter(const std::shared_ptr<BaseServiceAdapter> adapter) {
    if (adapter == nullptr) {
        return;
    }
    adapter_ = adapter->GetCastAdapter<AudioDectAdapterInterface>();
}

std::string AudioDectImpl::Description() {
    Log(DEBUG) << "AudioDectImpl::Description.";
    
    Log(DEBUG) << "**********************************************************";
    {
        std::shared_ptr<ServiceInterface> tmpService = adapter_->GetEngine()->CreateService<ServiceInterface>(Protocol(ServiceInterface));
        if (tmpService != nullptr) {
            tmpService->Init();
            tmpService->Description();
            tmpService->Test();
            tmpService->Destory();
        }}
    Log(DEBUG) << "**********************************************************";
    
    auto service = adapter_->GetEngine()->GetService<ServiceInterface>(Protocol(ServiceInterface));
    Log(DEBUG) << "AudioDectImpl::Description->" << service->GetTestString();
    
    return "AudioDectImpl";
}
