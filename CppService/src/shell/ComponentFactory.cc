/*
 * ComponentFactory.cc
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "ComponentFactory.h"
#include "ServiceBuilder.hpp"
#include "AudioDectBuilder.hpp"

#define ADD_SERVICE_BUILDER(_builder_) { \
    std::shared_ptr<BaseServiceBuilder> serviceBuild = std::make_shared<_builder_>(); \
    builders_->push_back(serviceBuild); \
}

void ComponentFactory::ConstructBuilders() {
    ADD_SERVICE_BUILDER(ServiceBuilder)
    ADD_SERVICE_BUILDER(AudioDectBuilder)
}

ComponentFactory::ComponentFactory() {
    builders_ = std::make_shared<std::vector<std::shared_ptr<BaseServiceBuilder>>>();
}