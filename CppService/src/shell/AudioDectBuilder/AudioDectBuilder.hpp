/*
 * AudioDectBuilder.h
 *
 * Created by ruibin.chow on 2023/07/12.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef AUDIODECTBUILDER_H
#define AUDIODECTBUILDER_H

#include "BaseServiceBuilder.h"
#include "BaseEngine.h"
#include "BaseService.h"
#include "AudioDectImpl.h"
#include "AudioDectAdapterImpl.hpp"

class AudioDectBuilder: public BaseServiceBuilder
{
public:
    AudioDectBuilder() = default;
    virtual ~AudioDectBuilder() = default;

public:
    std::shared_ptr<BaseService> BuildService() override {
        std::shared_ptr<BaseServiceAdapter> adapter = std::make_shared<AudioDectAdapterImpl>();
        adapter->SetEngine(engine_.lock());
        std::shared_ptr<AudioDectInterface> impl = std::make_shared<AudioDectImpl>();
        impl->SetAdapter(adapter);
        return impl;
    };

    std::string GetServiceName() override {
        return "AudioDectInterface";
    };
};

#endif /* !AUDIODECTBUILDER_H */
