/*
 *
 * Engine.cc
 *
 * Created by Ruibin.Chow on 2023/07/11.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "Engine.h"
#include "BaseService.h"

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
    } else {
        auto builder = componentFactory_->GetBuilder(name);
        if (builder != nullptr) {
            builder->SetEngine(this->shared_from_this());
            auto service = builder->BuildService();
            serviceMap_->emplace(name, service);
            return service;
        }
    }
    return nullptr;
}

void Engine::Init(void) {
    componentFactory_->ConstructBuilders();
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
    if (builder != nullptr) {
        componentFactory_->UpdateBuilder(builder);
    }
}


