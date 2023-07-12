/*
 * AudioDectInterface.h
 *
 * Created by ruibin.chow on 2023/07/12.
 * Copyright (c) 2023年 ruibin.chow All rights reserved.
 */

#ifndef AUDIODECTINTERFACE_H
#define AUDIODECTINTERFACE_H

#include <memory>
#include "BaseService.h"

class AudioDectInterface: public BaseService
{
public:
    virtual ~AudioDectInterface() = default;
};

#endif /* !AUDIODECTINTERFACE_H */
