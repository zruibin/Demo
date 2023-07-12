/*
 *
 * Engine.cc
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "Engine.h"
#include "ServiceBuilder.hpp"

Engine::Engine() {
    serviceMap_ = std::make_shared<std::unordered_map<std::string, 
                                       std::shared_ptr<BaseService>>>();
    componentFactory_ =  std::make_shared<ComponentFactory>();
}

Engine::~Engine() {
    
}

std::shared_ptr<BaseService> Engine::GetServiceByName(const std::string& name) {
    auto serviceIter = serviceMap_->find(name);
    if (serviceIter != serviceMap_->end()) {
        return serviceIter->second;
    }
    return nullptr;
}

void Engine::Init(void) {
    componentFactory_->ConstructBuilders();
    auto sharedBuilders = componentFactory_->GetBuilders();
    for (auto& builder : *sharedBuilders) {
        builder->SetEngine(this->shared_from_this());
        std::string name = builder->GetServiceName();
        std::shared_ptr<BaseService> service = builder->BuildService();
        service->Init();
        serviceMap_->emplace(name, service);
    }
}

void Engine::Destory(void) {
    if (serviceMap_ != nullptr) {
        for (auto& [key, service] : *serviceMap_) {
            service->Destory();
        }
        serviceMap_->clear();
        serviceMap_ = nullptr;
    }
}

void Engine::UpdateService(std::shared_ptr<BaseServiceBuilder> builder) {
    if (serviceMap_ != nullptr && builder != nullptr) {
        builder->SetEngine(this->shared_from_this());
        auto name = builder->GetServiceName();
        auto it = serviceMap_->find(name);
        if (it != serviceMap_->end()) {
            it->second = builder->BuildService();
        } else {
            serviceMap_->emplace(name, builder->BuildService());
        }
    }
}


