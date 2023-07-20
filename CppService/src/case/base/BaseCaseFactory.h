/*
 * BaseCaseFactory.h
 *
 * Created by Ruibin.Chow on 2023/07/20.
 * Copyright (c) 2023年 Ruibin.Chow All rights reserved.
 */

#ifndef BASECASEFACTORY_H
#define BASECASEFACTORY_H

class BaseCaseBuilder;

class BaseCaseFactory
{
public:
    virtual ~BaseCaseFactory() = default;
    
public:
    virtual void ConstructBuilders() = 0;
    virtual std::shared_ptr<BaseCaseBuilder> GetBuilder(const std::string&) = 0;
    virtual void UpdateBuilder(std::shared_ptr<BaseCaseBuilder>) = 0;
};

#endif /* !BASECASEFACTORY_H */
