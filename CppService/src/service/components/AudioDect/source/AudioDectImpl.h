/*
 * AudioDectImpl.h
 *
 * Created by ruibin.chow on 2023/07/12.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef AUDIODECTIMPL_H
#define AUDIODECTIMPL_H

#include "AudioDectInterface.h"
#include "AudioDectAdapterInterface.h"

class AudioDectImpl : public AudioDectInterface
{
public:
    explicit AudioDectImpl() = default;
    virtual ~AudioDectImpl() = default;
    
public:
    void Init() override;
    void Destory() override;
    void SetAdapter(const std::shared_ptr<BaseServiceAdapter>) override;
    const char* Description() override;
    
private:
    std::shared_ptr<AudioDectAdapterInterface> adapter_;
};

#endif /* !AUDIODECTIMPL_H */
