/*
 * ComponentFactory.cc
 *
 * Created by Ruibin.Chow on 2023/07/12.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#include "ComponentFactory.h"
#include "ServiceBuilder.hpp"
#include "LoggerBuilder.hpp"
#include "AudioDectBuilder.hpp"

void ComponentFactory::ConstructBuilders() {
    AddBuilder<ServiceBuilder>();
    AddBuilder<LoggerBuilder>();
    AddBuilder<AudioDectBuilder>();
}
