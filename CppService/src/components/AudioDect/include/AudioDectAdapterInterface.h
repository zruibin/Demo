/*
 * AudioDectAdapterInterface.h
 *
 * Created by ruibin.chow on 2023/07/12.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef AUDIODECTADAPTERINTERFACE_H
#define AUDIODECTADAPTERINTERFACE_H

#include "BaseServiceAdapter.h"

class AudioDectAdapterInterface: public BaseServiceAdapter
{
public:
    virtual ~AudioDectAdapterInterface() = default;
};

#endif /* !AUDIODECTADAPTERINTERFACE_H */